import argparse
import os
import json


def read_json_file(filename):
    with open(filename, 'r') as f:
        return json.load(f)


def gen_input_params(args):
    d_shukey = read_json_file(args.shukey_file)
    d_enclave_info = read_json_file(args.enclave_info_file)
    d_shukey_forward = read_json_file(args.shukey_forward_file)
    d_request_param = read_json_file(args.request_param_file)
    d = {
        'shu_info': {
            'shu_pkey': d_shukey['public-key'],
            'encrypted_shu_skey': d_shukey_forward['encrypted_skey'],
            'shu_forward_signature': d_shukey_forward['forward_sig'],
            'enclave_hash': d_enclave_info['enclave-hash'],
        },
        'parser_path': '../lib/hello.signed.so',
        'keymgr_path': '../lib/keymgr.signed.so',
        'parser_enclave_hash': d_enclave_info['enclave-hash'],
        'dian_pkey': args.dian_pkey,
        'param': {
            'param_data': d_request_param['encrypted-input'],
            'public-key': d_request_param['analyzer-pkey'],
        },
    }
    with open(args.output, 'w') as f:
        f.write(json.dumps(d))
    f.close()


def main():
    parser = argparse.ArgumentParser(description='Generate Fidelius Analyzer Input Parameters Script',
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--shukey-file', default='hello.key.json',
                        help='data analyzer shu key file')
    parser.add_argument('--enclave-info-file', default='info.json',
                        help='enclave information file')
    parser.add_argument('--dian-pkey', help='dian public key')
    parser.add_argument('--shukey-forward-file', default='hello.request.shukey.foward.json',
                        help='data analyzer encrypted shu private key file')
    parser.add_argument('--request-param-file',
                        default='hello_param.json', help='request parameters file')
    parser.add_argument(
        '--output', default='input_param.json', help='output file name')
    args = parser.parse_args()
    gen_input_params(args)


if __name__ == '__main__':
    main()
