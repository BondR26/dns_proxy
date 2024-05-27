Precondition (Cmake installed)
1. execute command : "chmod +x build.sh"
2. Run the ./build.sh script
3. Go to /build directory and make sure that config.dat is inside bin
4. Now, execute command : "./dns_proxy & disown" in order to start it and then be able to
5. continue on with shell
6. Here is the thing, i programmed my dns_proxy to receive client on localhost port 8080 
7. I suppose, there could have been other solutions(edit defualt dns server, set up dns server to be at 8080 port)
8. However in order to keep it simple and still to showcase my ability to solve tasks, its enought to receive the
9. client on port 8080;
10. to check how our proxy is working - run command : "dig @localhost -p 8080 google.com"
11. since ive put it in config.data in balcklist you should receive smth like : 
Received request from client
Request for google.com
The domain name google.com was found among blacklist
; <<>> DiG 9.11.3-1ubuntu1.7-Ubuntu <<>> @localhost -p 8080 google.com
; (1 server found)
;; global options: +cmd
;; connection timed out; no servers could be reached
12. After checking how it works , you have to find and kill the process 
13. use : "ps aux" to find the PID of ./dns_proxy;
14 use: "kill <PID>" to kill it; 