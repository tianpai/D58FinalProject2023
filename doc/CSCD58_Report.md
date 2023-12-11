| Name          | Student Number |
| ------------- | -------------- |
| Tianpai Zhang | 1005221332     |
| In Kim        | 1007757973     |
| Yining He     | 1006308979     |
# Description and explanation of the project, goals
This project is for us to create our own simplified version of a Virtual Private Network (VPN). A host can send and receive messages to different hosts. So whenever Host A sends a message, the packet is sent to the VPN server and the VPN server sends the message to the destination, and the message received goes to the VPN server and then to Host A.

Our goal for this project is to implement the following features of a VPN:
- Fragmentation
- Encapsulation
- Encryption
- Proxy server

# Contribution of each member of the team
- [ ] TP - insert contribution here.
# Instructions in how to run and test the implementation
Mininet is used to test our implementation. Please test on a machine with Mininet installed.
### Compile the project code: 
1. Navigate to the project source directory: `D58FinalProject2023/src`, where the `Makefile` is located. 
2. Run the command: `make` in the project source directory. Three executable files will be created: `vpn_client`, `vpn_source` and `vpn_destination`. These three executable files will be run on three different Mininet hosts to test our code.
### Start the custom Mininet topology:
1. Navigate to the `D58FinalProject2023/vpn_topo` directory, where the `simple_vpn.py` file is located.
2. Run the command: `sudo python simple_vpn.py` to start Mininet with our custom topology. Below is an image of our topology in MiniEdit (`vpn_topo/vpn_topology.png`).
![[Pasted image 20231210181102.png]]
### Run the demo files to test our implementation:
1. Inside Mininet, create external terminals for any three different hosts. For example:
	```
	mininet> xterm h1
	mininet> xterm h2
	mininet> xterm h3
	```
	These three hosts will be: the VPN user's computer (`client`); the VPN proxy server's computer (`server`); and the computer that's outside the VPN (`destination`), that the `client` wants to send packets to.
	
2. Choose one Mininet host to be the `destination`, say `h3`, and go to its external terminal. Navigate to the `D58FinalProject2023/src` directory, and run:
	```
	./vpn_destination h3
	```
3. Choose one host 
# Implementation details and documentation
See `D58FinalProject2023/doc/CSCD58_Documentation.txt` in the project repository for detailed documentation of our implementation.


# Analysis and discussion on the project, results and implementation

# Concluding remarks, lessons learned
Each of the team members has their own things to say here:

Tianpai Zhang:

In Kim:

Yining He:
