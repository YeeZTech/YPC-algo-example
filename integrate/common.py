#!/usr/bin/python3
import os
import sys
import subprocess
import json
import project


current_file = os.path.abspath(__file__)
integrate_dir = os.path.dirname(current_file)
example_dir = os.path.dirname(integrate_dir).replace(" ", "\ ")
example_bin = os.path.join(example_dir, "./bin")
example_lib = os.path.join(example_dir, "./lib")

ypc_lib = os.path.join(project.ypc_lib_dir(), "./")
ypc_bin = os.path.join(ypc_lib, "../bin")

kmgr_enclave = {
    'stdeth': os.path.join(ypc_lib, "keymgr.signed.so"),
    'gmssl': os.path.join(ypc_lib, "keymgr_gmssl.signed.so"),
}


def execute_cmd(cmd):
    print("execute_cmd: {}".format(cmd))
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
        raise RuntimeError('Failed to execute cmd {}'.format(cmd))
    return p.stdout.read().decode('utf-8', errors='ignore')


def fid_keymgr_create(user_id, crypto="stdeth"):
    cmd = os.path.join(ypc_bin, "./keymgr_tool")
    cmd = cmd + " --crypto {}".format(crypto)
    param = {"create": "", "user-id": user_id}
    for k, v in param.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_keymgr_list(crypto="stdeth"):
    cmd = os.path.join(ypc_bin, "./keymgr_tool")
    cmd = cmd + " --crypto {}".format(crypto)
    output = execute_cmd("{} --list".format(cmd))
    ls = output.split("\n")
    tkeyid = ''
    keys = {}

    for l in ls:
        l = l.strip()
        if l.startswith(">> key "):
            ks = l.split(":")
            tkeyid = ks[1].strip()
        if l.startswith("public key:"):
            ks = l.split(":")
            pkey = ks[1].strip()
            if pkey.startswith(tkeyid):
                keys[tkeyid] = pkey

    return keys


def get_keymgr_private_key(keyid, crypto_type="stdeth"):
    cmd = os.path.join(ypc_bin, "./keymgr_tool")
    cmd = cmd + " --crypto {}".format(crypto_type)
    output = execute_cmd("{} --list".format(cmd))
    ls = output.split("\n")
    ks = ls[0].split(' ')
    dir_ = ks[len(ks) - 1]
    fp = os.path.join(dir_, keyid)
    info = {}
    with open(fp) as of:
        info = json.load(of)
        return info['private_key']


def fid_keymgr(**kwargs):
    cmd = os.path.join(ypc_bin, "./keymgr_tool")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_data_provider(**kwargs):
    cmd = os.path.join(ypc_bin, "./data_provider")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_dump(**kwargs):
    cmd = os.path.join(ypc_bin, "./ydump")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_terminus(**kwargs):
    cmd = os.path.join(ypc_bin, "./yterminus")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def fid_analyzer(**kwargs):
    cmd = os.path.join(ypc_bin, "./fid_analyzer")
    cmd = "GLOG_logtostderr=1 " + cmd
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def iris_data(**kwargs):
    cmd = os.path.join(ypc_bin, "./iris_gen_classify_input")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]


def iris_model(**kwargs):
    cmd = os.path.join(ypc_bin, "./iris_gen_model")
    for k, v in kwargs.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = execute_cmd(cmd)
    return [cmd, output]
