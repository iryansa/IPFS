# InterPlanetary File System (IPFS) Simulation

This project simulates the core functionality of the InterPlanetary File System (IPFS) using a Distributed Hash Table (DHT) structure. The system allows content-addressable file sharing across a simulated distributed network using circular identifier spaces and hash-based storage.

## Features
- **Content Addressable Storage**: Files are identified and retrieved using their content's hash values.
- **Distributed Hash Table (DHT)**: Manages file storage and retrieval across a simulated multi-machine environment.
- **Efficient Search**: Supports logarithmic search complexity using routing tables.
- **Dynamic Network**: Machines can be added or removed dynamically with proper data redistribution.

## How to Run

1. **Setup Project**:
   - Open **Visual Studio**.
   - Create an **Empty C++ Project**.
   - Import all files from the `MainFiles` folder of this repository.

2. **Build Project**:
   - Build the project within Visual Studio.

3. **Install Graphviz**:
   - Ensure **Graphviz** is installed on your system. Add it to your system's dependencies if required. You can download and install Graphviz from <a href="https://graphviz.org/" target="_blank">Graphviz's official site</a>.

4. **Execute**:
   - Run the project and interact with the available commands to test the IPFS simulation features.

---

Feel free to customize the instructions further if required.
