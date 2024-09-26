from classic_job import classic_job
import os
import common


# test
if __name__ == "__main__":
    crypto = "stdeth"
    name = "imageblur"
    data = os.path.join(common.data_dir, "image/BaboonRGB.bmp")
    parser = os.path.join(common.lib_dir, "image_blur_parser.signed.so")
    plugin = os.path.join(common.lib_dir, "libimage_reader.so")
    input_param = "5,1.526"
    output_file = os.path.join(common.data_dir, "image_out/BaboonRGB-10.jpg")

    cj = classic_job(crypto, name, data, parser, plugin, input_param, {
        'remove-files': True,
        'isImage': True
    }, {
        'width': 512,
        'height': 512,
        'channels': 3,
        'output_file': output_file
    })
    cj.run()

    print("result is : ", cj.result)
