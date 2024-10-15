from multistream_job import multistream_job
import json
import os
import common


def psi():
    name = "psi"
    crypto = "stdeth"
    data_a = os.path.join(common.example_dir, "./dataset/a.csv")
    data_b = os.path.join(common.example_dir, "./dataset/b.csv")
    data = [data_a, data_b]
    parser = os.path.join(common.example_lib, "psi_parser.signed.so")
    plugin = {
        data_a: os.path.join(common.example_lib, "liba_csv_reader.so"),
        data_b: os.path.join(common.example_lib, "libb_csv_reader.so"),
    }
    input_param = "hah"
    cj = multistream_job(crypto, name, data, parser, plugin, input_param, {})
    cj.run()
    print("result is : ", cj.result)


def main():
    psi()


if __name__ == "__main__":
    main()
