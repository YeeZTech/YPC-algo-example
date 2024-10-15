from multistream_job import multistream_job
import json
import os
import common


def pir():
    name = "pir"
    crypto = "stdeth"
    data_business = os.path.join(common.example_dir, "./dataset/business.csv")
    data = [data_business]
    parser = os.path.join(common.example_lib, "pir_parser.signed.so")
    plugin = {
        data_business: os.path.join(common.example_lib, "libbusiness_csv_reader.so"),
    }
    input_param = "BDCQ.SF1AA2CS,2017.09"
    cj = multistream_job(crypto, name, data, parser, plugin, input_param, {})
    cj.run()
    print("result is : ", cj.result)


def main():
    pir()


if __name__ == "__main__":
    main()
