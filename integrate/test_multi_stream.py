from multistream_job import multistream_job
import json
import os
import common


def gen_data(**kwargs):
    cmd = os.path.join(common.example_bin, "./gen_data")
    output = common.execute_cmd(cmd)
    return [cmd, output]


def gen_param(name, input_param):
    output_file = "{}.param".format(name)
    param = {
        "param": input_param,
        "output": output_file,
    }
    cmd = os.path.join(common.example_bin, "./gen_param")
    for k, v in param.items():
        cmd = cmd + " --{} {}".format(k, v)
    output = common.execute_cmd(cmd)
    with open(output_file) as fp:
        info = json.load(fp)
        return info['param']


if __name__ == "__main__":
    name = "multi_stream"
    gen_data()

    crypto = "stdeth"
    data = ["id_mapping.sealed", "sdk.sealed"]
    parser = os.path.join(common.example_lib, "tel_mapping.signed.so")
    plugin = os.path.join(common.example_lib, "libblockfile_reader.so")

    use_js = True
    input_param = gen_param(name, "13000000002")
    print(input_param)

    cj = multistream_job(crypto, name, data, parser, plugin, input_param, {
        'request-use-js': use_js,
    })

    cj.run()

    print("result is : ", cj.result)
