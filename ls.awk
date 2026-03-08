BEGIN {
    send = 0
    recv = 0
    dropped = 0
    rout = 0
}

# Sent CBR packets from node 0 or 1
$1 == "+" && ($3 == "0" || $3 == "1") && $5 == "cbr" {
    send++
}

# Received CBR packets at node 3
$1 == "r" && $4 == "3" && $5 == "cbr" {
    recv++
}

# Dropped packets
$1 == "d" {
    dropped++
}

# Routing packets (Link State)
$1 == "r" && $5 == "rtProtoLS" {
    rout++
}

END {
    print "Performance Evaluation (Link State Routing)"
    print "-----------------------------------------"
    print "No of packets sent       : ", send
    print "No of packets received   : ", recv
    print "No of packets dropped    : ", dropped
    print "No of routing packets    : ", rout

    if (recv > 0) {
        NOH = rout / recv
    } else {
        NOH = 0
    }

    if (send > 0) {
        PDR = recv / send
    } else {
        PDR = 0
    }

    print "Normalized Overhead      : ", NOH
    print "Packet Delivery Ratio    : ", PDR
}