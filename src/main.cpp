
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <Session_3_Project_inferencing.h>

Adafruit_MPU6050 mpu;

float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t bufferIndex = 0;

const float CONFIDENCE_THRESHOLD = 0.60f;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(8, 9);

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found!");
    while (true) {
      delay(1000);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  Serial.println("Session 4: TinyML model deployment");
  Serial.println("Classes: idle, shake, wave");
  Serial.println("Collecting sensor data...");
}

void loop() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  if (bufferIndex + 3 <= EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
    buffer[bufferIndex++] = a.acceleration.x;
    buffer[bufferIndex++] = a.acceleration.y;
    buffer[bufferIndex++] = a.acceleration.z;
  }

  if (bufferIndex >= EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
    signal_t signal;
    numpy::signal_from_buffer(
      buffer,
      EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE,
      &signal
    );

    ei_impulse_result_t result = {0};

    unsigned long startTime = micros();
    EI_IMPULSE_ERROR status = run_classifier(&signal, &result, false);
    unsigned long inferenceTime = micros() - startTime;

    Serial.printf("Inference time: %.3f ms\n",
                  inferenceTime / 1000.0f);

    if (status == EI_IMPULSE_OK) {
      size_t bestIndex = 0;
      float bestConfidence = 0.0f;

      for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        Serial.printf("%s: %.3f\n",
                      result.classification[i].label,
                      result.classification[i].value);

        if (result.classification[i].value > bestConfidence) {
          bestConfidence = result.classification[i].value;
          bestIndex = i;
        }
      }

      if (bestConfidence >= CONFIDENCE_THRESHOLD) {
        Serial.printf("Prediction: %s (%.1f%%)\n",
                      result.classification[bestIndex].label,
                      bestConfidence * 100.0f);
      } else {
        Serial.println("Prediction uncertain — below threshold");
      }
    } else {
      Serial.printf("Classifier error: %d\n", status);
    }

    Serial.println("--------------------");
    bufferIndex = 0;
  }

  delay(20);
}
