#myflooding_wireless.tcl
#This file configures all network parameters and network topology

#Ted 05/13/2014

set val(chan)           Channel/WirelessChannel    ;#Channel Type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac) 		Mac/802_11                 ;# MAC type
set val(ifq)            CMUPriQueue    ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                         ;# max packet in ifq
set val(rp)             DumbAgent	           ;# routing protocol 
set val(nn) 		3
set val(x)              200
set val(y)              200

puts "hello world111~~~~"
set ns [new Simulator]

set tracefd [open wireless-myflooding.tr w]
$ns trace-all $tracefd
set namtrace [open wireless-myflooding.nam w]
$ns namtrace-all-wireless $namtrace $val(x) $val(y)

#$ns use-newtrace

set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)
create-god $val(nn)

set chan_1_ [new $val(chan)]

$ns node-config -adhocRouting 	$val(rp) \
                -llType 	$val(ll) \
                -macType 	$val(mac) \
                -ifqType 	$val(ifq) \
                -ifqLen 	$val(ifqlen) \
                -antType 	$val(ant) \
                -propType 	$val(prop) \
                -phyType 	$val(netif) \
                -topoInstance 	$topo \
                -agentTrace 	ON \
                -routerTrace 	ON \
                -macTrace 	OFF \
                -movementTrace 	OFF \
                -channel 	$chan_1_ 

puts "hello world2222~~~~"
# create nodes
for {set i 0} {$i < $val(nn)} {incr i} {
    set n($i) [$ns node]
    $n($i) set Y_ [expr 100*$i + 100]
    $n($i) set X_ [expr 100*$i + 200]
    $n($i) set Z_ 0.0
}

######################################################

# attach a new Agent/MYFLOODING
puts "hello world3333"
for {set i 0} {$i < $val(nn)} {incr i} {
    set a($i) [new Agent/MYFLOODING]
    $n($i) attach $a($i)
}

puts "hello world444444444444"

# #set nodes information
for {set i 0} {$i < $val(nn)} {incr i} {
	$a($i) set-addr $i
}

puts "hello world55555555555555"
# set up some events

#send_packet
$ns at 0.2 "$a(0) send_my_message"

$ns at 1.0 "finish"

proc finish {} {
        global ns tracefd namtrace 
        $ns flush-trace
	close $tracefd
        close $namtrace

	exec nam wireless-myflooding.nam &
        exit 0
}

$ns run

