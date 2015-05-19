## Installation
1. _Mininet VM_
  * Download the [official Mininet VM](https://github.com/mininet/mininet/wiki/Mininet-VM-Images "Mininet VM Images"). Make sure you have Mininet version 2.1.0. 
  * Prepare VM  
        ```
        $ sudo apt-get install python-dev python-pip screen
        ```
2. _Quagga_  
        ```
        $ sudo apt-get install quagga
        ```
3. _MiniNExT_
  * Make sure that Mininext’s dependencies are installed.  
        ```
        $ sudo apt-get install help2man python-setuptools
        ```
  * Clone miniNExT and install it.  
        ```
        $ git clone https://github.com/USC-NSL/miniNExT.git miniNExT/  
        $ cd miniNExT  
        $ git checkout 1.4.0  
        $ sudo make install  
        ```
4. _Requests_  
        ```
        $ sudo pip install requests
        ```
5. _aSDX_
  * Clone aSDX.  
        ```
        $ cd ~  
        $ git clone https://github.com/nsg-ethz/supercharged_sdx.git asdx/ 
        $ chmod 755 ~/asdx/xrs/client.py ~/asdx/xrs/route_server.py ~/asdx/examples/simple/mininet/sdx_mininext.py  
        ```
6. _ExaBGP_ (tested with version 3.4.10)  
        ```
        $ sudo pip install -U exabgp  
        ```
7. _Ryu_
  * Clone Ryu  
        ```
        $ cd ~  
        $ git clone git://github.com/osrg/ryu.git  
        ```
  * Before installing it, replace flags.py with the provided file
        ```
        $ cp ~/asdx/ryu/flags.py ~/ryu/ryu/flags.py
        $ cd ryu
        $ sudo python ./setup.py install
        ```
  * Dependencies
        ```
        $ sudo apt-get install python-routes  
        $ sudo pip install oslo.config --upgrade  
        $ sudo pip install msgpack-python  
        $ sudo pip install eventlet  
        ```  

## Usage
1. Mininet  
        ```
        $ cd ~/asdx/examples/simple/mininet/  
        $ sudo ./sdx_mininext.py  
        ```
2. Make OVS use OpenFlow 1.3  
        ```
        $ sudo ovs-vsctl set bridge s1 protocols=OpenFlow13
        ```
3. Start Ryu - The Controller  
        ```
        $ ryu-manager ~/asdx/ctrl/asdx.py --asdx-dir simple
        ```
4. Start the Route Server  
        ```
        $ cd ~/asdx/xrs/
        $ sudo ./route_server.py simple
        ```
5. Start ExaBGP  
        ```
        $ exabgp /home/mininet/asdx/examples/test/controller/sdx_config/bgp.conf
        ```
6. Remove old RIBs  
        ```
        $ rm ~/asdx/xrs/ribs/172.0.0.* 
        ```
    
## Run the "simple" Example
1. Check if the route server has correctly advertised the routes  
        ```
        mininext> a1 route -n  
	Kernel IP routing table  
	Destination     Gateway         Genmask         Flags Metric Ref    Use Iface  
	140.0.0.0       172.0.1.3       255.255.255.0   UG    0      0        0 a1-eth0  
	150.0.0.0       172.0.1.4       255.255.255.0   UG    0      0        0 a1-eth0  
	172.0.0.0       0.0.0.0         255.255.0.0     U     0      0        0 a1-eth0  
	```
2. Testing the Policies  
  * The participants have specified the following policies:  
      * _Participant A - outbound:_
        ```
        matcht(dstport=80) >> fwd(B) + match(dstport=4321/4322) >> fwd(C)
    	```
	  * _Participant C - inbound:_
		```
    	match(dstport = 4321) >>  fwd(C1) + match(dstport=4322) >> fwd(C2)
		``` 
  * Starting the  `iperf` servers:  
        ```
        mininext> b1 iperf -s -B 140.0.0.1 -p 80 &  
        mininext> c1 iperf -s -B 140.0.0.1 -p 4321 &  
        mininext> c2 iperf -s -B 140.0.0.1 -p 4322 &  
        ```
  * Starting the  `iperf` clients:  
        ```
        mininext> a1 iperf -c 140.0.0.1 -B 100.0.0.1 -p 80 -t 2  
        mininext> a1 iperf -c 140.0.0.1 -B 100.0.0.1 -p 4321 -t 2  
        mininext> a1 iperf -c 140.0.0.1 -B 100.0.0.1 -p 4322 -t 2  
        ```
  * Successful `iperf` connections should look like this:  
        ```
        mininext> c2 iperf -s -B 140.0.0.1 -p 4322 &  
        mininext> a1 iperf -c 140.0.0.1 -B 100.0.0.1 -p 4322 -t 2  
        ------------------------------------------------------------  
        Client connecting to 140.0.0.1, TCP port 4322  
        Binding to local address 100.0.0.1  
        TCP window size: 85.3 KByte (default)  
        ------------------------------------------------------------  
        [  3] local 100.0.0.1 port 4322 connected with 140.0.0.1 port 4322  
        [ ID] Interval       Transfer     Bandwidth  
        [  3]  0.0- 2.0 sec  1.53 GBytes  6.59 Gbits/sec  
        ```
  * In case the `iperf` connection is not successful, you should see the message, `connect failed: Connection refused.`
