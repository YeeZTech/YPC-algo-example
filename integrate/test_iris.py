from classic_job import classic_job
import os
import common


# test jenkins
if __name__ == "__main__":
    # should also set template param (Crypto) of iris_parser
    # and update iris_parser.signed.so by compiling the source code
    crypto = "stdeth"

    name = "iris"
    data = os.path.join(common.example_bin, "iris.data")
    parser = os.path.join(common.example_lib, "iris_parser.signed.so")
    plugin = os.path.join(common.example_lib, "libiris_reader.so")
    input_param = "123"
    cj = classic_job(crypto, name, data, parser, plugin, input_param, {
        'remove-files': True
    })
    cj.run()

    print("result is : ", cj.result)
