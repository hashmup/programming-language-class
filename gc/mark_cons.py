#!/usr/bin/python
# -*- coding: utf-8 -*-
import re,sys

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

def mark_cons_all(fp):
    live = 0
    for t,kind,addr,sz in analyze_log(fp):
        if kind == "born":
            live += sz
            if t != live:
                print t, (1.0 * live / (t - live))
        elif kind == "dead":
            live -= sz
        else:
            assert 0

def mark_cons_y(fp):
    birth_time = {}
    histogram = {}
    total_cnt = 0.0
    for t,kind,addr,sz in analyze_log(fp):
        if kind == "born":
            assert addr not in birth_time
            birth_time[addr] = t
        elif kind == "dead":
            assert addr in birth_time
            life_time = t - birth_time[addr]
            del birth_time[addr]
            histogram[life_time] = histogram.get(life_time, 0) + sz
            total_cnt = total_cnt + sz
        else:
            assert 0
    sum_cnt = 0.0
    for life_time,count in sorted(histogram.items()):
        sum_cnt = sum_cnt + count
        if total_cnt != sum_cnt:
            print life_time, (1.0 * sum_cnt / (total_cnt - sum_cnt))


def main():
    mark_cons_all(sys.stdin)
    # mark_cons_y(sys.stdin)

main()
