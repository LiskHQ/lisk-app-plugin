import os
import re
import json
from pathlib import Path
from typing import Optional

from web3 import Web3
from eth_typing import ChainId

from ledger_app_clients.ethereum.utils import get_selector_from_data, recover_transaction
from ragger.navigator import NavInsID

from ledger_app_clients.ethereum.client import EthAppClient
import ledger_app_clients.ethereum.response_parser as ResponseParser

DERIVATION_PATH = "m/44'/60'/0'/0/0"
makefile_relative_path = "../Makefile"

makefile_path = (Path(os.path.dirname(os.path.realpath(__file__))) / Path(makefile_relative_path)).resolve()

pattern = r'.*APPNAME.*=.*'

default_strip_parameter = " \t\n\r\x0b\x0c"

ROOT_SCREENSHOT_PATH = Path(__file__).parent
ABIS_FOLDER = "%s/abis" % (os.path.dirname(__file__))

def get_appname_from_makefile() -> str:
    '''
    Parse the app Makefile to automatically get the APPNAME value
    '''
    APPNAME: Optional[str] = None
    with open(makefile_path) as file:
        for line in file:
            if re.search(pattern, line):
                _, var = line.partition("=")[::2]
                APPNAME = var.strip(default_strip_parameter + '"')

    if APPNAME is None:
        raise AssertionError("Unable to find APPNAME in the Makefile")

    return APPNAME

PLUGIN_NAME = get_appname_from_makefile()

def load_contract(address, abi_file):
    with open("%s/%s.abi.json" % (ABIS_FOLDER, abi_file)) as file:
        return Web3().eth.contract(
            abi=json.load(file),
            # Get address from filename
            address=bytes.fromhex(
                address
            )
        )

def run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr, value=0):
    client = EthAppClient(backend)

    # first setup the external plugin
    client.set_external_plugin(PLUGIN_NAME,
                               contract.address,
                               # Extract function selector from the encoded data
                               get_selector_from_data(data))

    tx_params = {
        "nonce": 20,
        "maxFeePerGas": Web3.to_wei(145, "gwei"),
        "maxPriorityFeePerGas": Web3.to_wei(1.5, "gwei"),
        "gas": 173290,
        "to": contract.address,
        "value": value,
        "chainId": ChainId.ETH,
        "data": data
    }

    # send the transaction
    with client.sign(DERIVATION_PATH, tx_params):
        # Validate the on-screen request by performing the navigation appropriate for this device
        if firmware.is_nano:
            navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                      [NavInsID.BOTH_CLICK],
                                                      "Accept",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)
        else:
            navigator.navigate_until_text_and_compare(NavInsID.USE_CASE_REVIEW_TAP,
                                                      [NavInsID.USE_CASE_REVIEW_CONFIRM,
                                                       NavInsID.USE_CASE_STATUS_DISMISS],
                                                      "Hold to sign",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)
    # verify signature
    vrs = ResponseParser.signature(client.response().data)
    addr = recover_transaction(tx_params, vrs)
    assert addr == wallet_addr.get()

class WalletAddr:
    client: EthAppClient

    def __init__(self, backend):
        self.client = EthAppClient(backend)

    def get(self, path=DERIVATION_PATH) -> bytes:
        with self.client.get_public_addr(display=False, bip32_path=path):
            pass
        return ResponseParser.pk_addr(self.client.response().data)[1]
