from classic_job import classic_job
import os
import common


# test jenkins
if __name__ == "__main__":
    # should also set template param (Crypto) of iris_parser
    # and update iris_parser.signed.so by compiling the source code
    crypto = "stdeth"

    name = "VIEM"
    data = os.path.join(common.example_data, "image/BaboonRGB.bmp")
    parser = os.path.join(common.example_lib, "viem_parser.signed.so")
    plugin = os.path.join(common.example_lib, "libimage_reader.so")
    input_param = "123"
    output_file = os.path.join(common.example_data, "image_out/BaboonRGB.jpg")

    cj = classic_job(crypto, name, data, parser, plugin, input_param, {
        'remove-files': False,
        'isImage': True
    }, {
        'width': 512,
        'height': 512,
        'channels': 3,
        'output_file': output_file
    })
    cj.run()

    print("result is : ", cj.result)
