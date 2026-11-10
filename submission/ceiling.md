# My ceiling — person detection on the ESP32-S3

## Vision experiment results

| Image size | My prediction | Measured time | FPS | Accuracy |
|---|---:|---:|---:|---:|
| 96 × 96 | Guess: Not recorded | 612 ms | 1.63 | 81.1% |
| 160 × 160 | 1713.6 ms | 1053 ms | 0.95 | 83.2% |
| 48 × 48 | 153 ms | 160 ms | 6.25 | 66.1% |

## FPS calculation

FPS = 1000 / measured inference time (ms)

## On-device performance

| Image size | Peak RAM usage | Flash usage |
|---|---:|---:|
| 96 × 96 | 122.8 KB | 279.9 KB |
| 160 × 160 | 236.6 KB | 295.8 KB |
| 48 × 48 | 90.1 KB | 289.4 KB |

## Experiment observations

The 160 × 160 model achieved the highest validation accuracy (83.2%), but it was the slowest and used the most RAM.

The 48 × 48 model was the fastest and used the least RAM, but its validation accuracy was the lowest (66.1%).

The 96 × 96 model provided a balance between speed, memory usage and accuracy.

## Which budget breaks first — time or memory?

1. A doorbell: **time** — the 96 × 96 model takes 612 ms, leaving only 388 ms within the one-second limit for the rest of the process.

2. A people counter: **memory** — one 320 × 240 RGB image requires 230,400 bytes (about 225 KiB) of memory.

3. Our team's project: **time** — the 96 × 96 model takes 612 ms per inference, and the 160 × 160 model takes 1053 ms. Inference speed is a practical limitation for our model.