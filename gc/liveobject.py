#!/usr/bin/python
# -*- coding: utf-8 -*-
import re,sys

# 以下のようなフォーマットの標準入力
# を解析して，オブジェクトの寿命の分布
# (gnuplot)を出力する．
#
# born: 12808160 24 bytes
# dead: 12808160
# born: 12808128 24 bytes
# born: 12808096 24 bytes
# dead: 12808096
# dead: 12808128
# born: 12808064 24 bytes
# born: 12808032 24 bytes
# born: 12808000 24 bytes
#
# 使用例(上記をa.txtとして):
#
# $ python lifetime_distribution.py < a.txt > lifetime.txt
# $ gnuplot
# gnuplot> plot "lifetime.txt"
#

born_exp = re.compile("born: (?P<addr>[0-9]+) (?P<sz>[0-9]+) bytes")
dead_exp = re.compile("dead: (?P<addr>[0-9]+)")

def analyze_log(fp):
    t = 0                       # time (bytes allocated)
    events = []
    sz_of_addr = {}
    for line in fp:
        m = born_exp.match(line)
        if m:
            addr = int(m.group("addr"))
            sz = int(m.group("sz"))
            t += sz
            assert addr not in sz_of_addr
            sz_of_addr[addr] = sz
            events.append((t, "born", addr, sz))
            continue
        m = dead_exp.match(line)
        if m:
            addr = int(m.group("addr"))
            assert addr in sz_of_addr
            sz = sz_of_addr[addr]
            del sz_of_addr[addr]
            events.append((t, "dead", addr, sz))
            continue
    return events

def show_live_data(fp):
    live = 0
    for t,kind,addr,sz in analyze_log(fp):
        if kind == "born":
            live += sz
            print t,live
        elif kind == "dead":
            live -= sz
            print t,live
        else:
            assert 0

def show_lifetime(fp):
    birth_time = {}
    histogram = {}
    for t,kind,addr,sz in analyze_log(fp):
        if kind == "born":
            assert addr not in birth_time
            birth_time[addr] = t
        elif kind == "dead":
            assert addr in birth_time
            life_time = t - birth_time[addr]
            del birth_time[addr]
            histogram[life_time] = histogram.get(life_time, 0) + sz
        else:
            assert 0
    for life_time,count in sorted(histogram.items()):
        print life_time,count


def main():
    show_live_data(sys.stdin)
    # show_lifetime(sys.stdin)

main()
