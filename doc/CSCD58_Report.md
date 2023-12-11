| Name          | Student Number |
| ------------- | -------------- |
| Tianpai Zhang | 1005221332     |
| In Kim        | 1007757973     |
| Yining He     | 1006308979     |
# Description and explanation of the project, goals
This project is for us to create our own simplified version of a Virtual Private Network (VPN). The features include: packet encapsulation and encryption to create a secure tunnel between client and server, and proxying to browse privately. 

A VPN is best explained with a case study. When the client wishes to send a packet to a host outside of the VPN, the client first encapsulates and encrypts the packet destined to the VPN server. The server, upon receiving the encapsulated packet from the client, decapsulates and decrypts it. The VPN server then forwards the packet to the outside host, in the client's name. The server does this by updating the source IP address of the client's packet to be the VPN server's IP address. A similar process happens when the outside host sends a reply back to the VPN server. The VPN server will forward the response back to the client, after encrypting and encapsulating. As a result, the client can communicates with the VPN server through a virtually private network because the data being communicated is encrypted. When the client sends a packet to a host outside the virtual private network, the VPN server acts like a proxy for the client. Which keeps the client's IP address secrete from the outside hosts that the client visits, and keeps the client's identity private.

Our goal for this project is to implement the following features of a VPN:
- **Encapsulation**: wrapping the client's packet, using the GRE protocol. This directs the packet to the VPN server to use the VPN server as a proxy.
- **Encryption**: 
- Proxy server
- Authentication
# Contribution of each member of the team
Generated contribution percentage from git: [link to the file](../contrib/contrib_report.txt)
Path to file: `D58FinalProject2023/contrib/contrib_report.txt`
##### Tianpai Zhang (35%)
- Encryption
- Create project Mininet topology
- Experiment with SSL/TLS
##### In Kim (35%)
- Encapsulation with GRE
- Send packet
- Receive packet
- Socket programming to send and receive packets
- VPN client & server demo file
##### Yining He (30%)
- Destination host demo file
- Format print packet
- Experiment with sending packets down raw socket
# Instructions in how to run and test the implementation
Mininet is used to test our implementation. Please test on a machine with Mininet installed.
### Compile the project code: 
1. Navigate to the project source directory: `D58FinalProject2023/src`, where the `Makefile` is located. 
2. Run the command: `make` in the project source directory. Three executable files will be created: `vpn_client`, `vpn_source` and `vpn_destination`. These three executable files will be run on three different Mininet hosts to test our code.
### Start the custom Mininet topology:
1. Navigate to the `D58FinalProject2023/vpn_topo` directory, where the `simple_vpn.py` file is located.
2. Run the command: `sudo python simple_vpn.py` to start Mininet with our custom topology. Below is an image of our topology in MiniEdit (`vpn_topo/vpn_topology.png`).
![[vpn_topology.png]]
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
	The command line argument `h3` is the host name that runs `vpn_destination`.

3. Choose one Mininet host to be the `server`, say `h2`, and go to its external terminal. Navigate to the `D58FinalProject2023/src` directory, and run:
	```
	./vpn_server h2
	```
	The command line argument `h2` is the host name that runs `vpn_server`.

4. Choose one Mininet host to be the e`client`, say `h1`, and go to its external terminal. Navigate to the `D58FinalProject2023/src` directory, and run:
	```
	./vpn_client h1 h2
	```
	The first command line argument `h1` is the host name that runs `vpn_client`. 
	The second command line argument `h2` is the host name of the VPN proxy server, which is the host name that runs `vpn_server`.
	
5. In external terminal of `h1` (or the host that ran `vpn_client`), enter to the prompt for the destination name. In our example, the destination host name is `h3`, the host name that ran `vpn_destination`.

6. watch the print statements across the three terminals. 
   Our demo code will perform the following:
	- The client will create an IP - TCP packet with a message as payload.
	- The client encapsulates 
# Implementation details and documentation
See `D58FinalProject2023/doc/CSCD58_Documentation.txt` in the project repository for detailed documentation of our implementation. Detailed doc-strings are found within the code as well.

For the encapsulation feature, we implemented the standard Generic Routing Encapsulation (GRE) protocol. `encap.c` and `decap.c` houses a pair of functions to perform the encapsulation and decapsulation. 



The `protocol.h` file houses useful constants like: the maximum payload size, common TCP flags, and IP version number. It also houses the custom structs for the GRE, IP and TCP headers. These components are useful for us to generate packet headers.

Commonly used utility functions such as computing and verifying checksum, parsing ip addresses, and printing packet info, are organized in `utils.c`, `host_info.c`, and `packet.c`, respectively.
# Analysis and discussion on the project results and implementation
We under-delivered on the features of the VPN. There are features we've discussed to implement, like: client authentication, sending raw packets down a raw socket with the SOCK_RAW option, 
# Concluding remarks, lessons learned
Implementing something low level like this presented unique challenges.
Each of the team members has their own things to say here:

##### Some lessons we learned:
One feature of the C programming language we learned is *serialization*. C structs with a mixture of datatypes of different sizes may contain unused spaces when stored in memory. This is to align the memory addresses of certain fields of a struct to the memory partition. To combat this, we

We learned that there 