# Create simulator
set ns [new Simulator]

# Enable Link State routing
$ns rtproto LS

# Trace files
set nf [open ls1.tr w]
$ns trace-all $nf

set nr [open ls2.nam w]
$ns namtrace-all $nr

# Finish procedure
proc finish {} {
    global ns nf nr
    $ns flush-trace
    close $nf
    close $nr
    exec nam ls2.nam &
    exit 0
}

# Create nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

# Create links
$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n0 1Mb 10ms DropTail

# UDP + CBR from n0 to n3
set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0

set null0 [new Agent/Null]
$ns attach-agent $n3 $null0

$ns connect $udp0 $null0

set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set packetSize 500
$cbr0 set interval 0.005

# UDP + CBR from n1 to n3
set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1

set null1 [new Agent/Null]
$ns attach-agent $n3 $null1

$ns connect $udp1 $null1

set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packetSize 500
$cbr1 set interval 0.005

# Start and stop traffic
$ns at 0.1 "$cbr1 start"
$ns at 0.2 "$cbr0 start"

$ns at 45.0 "$cbr1 stop"
$ns at 45.1 "$cbr0 stop"

$ns at 50.0 "finish"

# Run simulation
$ns run