# IoT Threat Model

**Device:** Home Wi-Fi router

**Asset:** The main assets are the router's administrator login, firmware, configuration settings, and the network traffic passing through it. The router is important because all connected devices depend on it. If someone compromises it, they could redirect traffic, change DNS settings, weaken firewall protection, or use it as part of a botnet.

**Threat:** The most realistic threat is automated scanners and botnets searching the internet for routers with default passwords, outdated firmware, or exposed administrator pages. Attackers can use compromised routers for activities such as DDoS attacks or traffic redirection. I do not know whether my router has these weaknesses or how often it is targeted, so this is a potential risk rather than a confirmed vulnerability.

**Response:** I would change the default administrator password, enable automatic firmware updates, prevent administrator access from the internet, and disable UPnP if it is not needed. I would also consider using a guest network for smart-home devices. These measures are practical and reduce common security risks without requiring expensive or complicated security systems.