from pathlib import Path
import json
import os
import datetime

from hexbytes import HexBytes

from web3 import Web3
from eth_typing import ChainId

from ledger_app_clients.ethereum.client import EthAppClient
import ledger_app_clients.ethereum.response_parser as ResponseParser
from ledger_app_clients.ethereum.utils import get_selector_from_data, recover_transaction
from ragger.navigator import NavInsID

from .utils import get_appname_from_makefile, DERIVATION_PATH


ROOT_SCREENSHOT_PATH = Path(__file__).parent
ABIS_FOLDER = "%s/abis" % (os.path.dirname(__file__))

PLUGIN_NAME = get_appname_from_makefile()


with open("%s/0xD7BE2Fd98BfD64c1dfCf6c013fC593eF09219994.abi.json" % (ABIS_FOLDER)) as file:
    contract = Web3().eth.contract(
        abi=json.load(file),
        # Get address from filename
        address=bytes.fromhex(os.path.basename(file.name).split(".")[0].split("x")[-1])
    )

def test_claim_regular_account(backend, firmware, navigator, test_name, wallet_addr):
    client = EthAppClient(backend)

    data = contract.encodeABI("claimRegularAccount", [
        [
            bytes.fromhex("9ca797d905e78f38685b61f62521632ef486bc6ce3e707d2af41b7fe146303c1"),
            bytes.fromhex("68b19fe3cb4ae3cd09faa5223373d2cc907c438d3b587c0d47034e848d4707e8"),
            bytes.fromhex("a10202c7b3db94d566f0d5e198ad0b5de39e983e95cf80afec25836851759d00"),
            bytes.fromhex("5ac0e1f56c2bbf81c101d84d50c2bcb5b45ec8852feb202c692a0f19f109b2d6"),
            bytes.fromhex("151bced47affdef60d90d68349af1228c9341fb900956f30dd5ebfc5cddf89ef"),
            bytes.fromhex("6f3f752b8381917128bc5145128af0d83e3d7547c83a2d28b20bb04b8952f4a6")
        ],
        bytes.fromhex("f83c23fb2532ad248e107ebd5312f1f78f742488fdec815c94d6b63aa7682eb6"),
        86511774,
        bytes.fromhex("00bb7b59073f45177edf430090d5aa3c44aa7f23"),
        (
            bytes.fromhex("664272cc25b0d20adf551b7a015795b9ad4ab23a4a19a509642bc042c00e5256"),
            bytes.fromhex("f10e444218d90bedfce6ac372c6b231f11fc44e0f6b230e8217f2aa8b8737e09")
        )
    ])

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
        "value": Web3.to_wei(0.1, "ether"),
        "chainId": ChainId.ETH,
        "data": data
    }

    # send the transaction
    with client.sign(DERIVATION_PATH, tx_params):
        # Validate the on-screen request by performing the navigation appropriate for this device
        if firmware.device.startswith("nano"):
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

def test_claim_multisig_account(backend, firmware, navigator, test_name, wallet_addr):
    client = EthAppClient(backend)

    data = contract.encodeABI("claimMultisigAccount", [
        [
            bytes.fromhex("f90c3db9f834e9a33086969313a1f5aa2288933e69a065cb829ab7524f113340"),
            bytes.fromhex("de50aabc4bf2cc38377047ec9d782e2cd01b734b455c3ce25491d04b6fef9a78"),
            bytes.fromhex("5c6039a4b7dce3d6ee659b5fb7f68f3a6a4d1a85e45e0cd85fb53213cf730745"),
            bytes.fromhex("670133dd4a82f56a113153314a77d1e68d117fb2cc72c0ee70b00988fe76c0bb"),
            bytes.fromhex("629779421eb7409cb867a7d8ee45aadc89a8b86f24423b737e1430832a78f026")
        ],
        bytes.fromhex("77abc2dd8ca5021dbb8e91e63f574b1e440764d2"),
        4915507196,
        (
            [
            ],
            [
            ]
        ),
        bytes.fromhex("77abc2dd8ca5021dbb8e91e63f574b1e440764d2"),
        [
            (
                bytes.fromhex("a141f1603dbcef26a6d4c83e2ba5d4383aee686f7c7c276e6c4a735ff37535db"),
                bytes.fromhex("d1d6091ff1adb3f043454dc274235f1df8dc051db890296e660d58f8b30ed30e")
            ),
        ]
    ])

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
        "value": Web3.to_wei(0.1, "ether"),
        "chainId": ChainId.ETH,
        "data": data
    }

    # send the transaction
    with client.sign(DERIVATION_PATH, tx_params):
        # Validate the on-screen request by performing the navigation appropriate for this device
        if firmware.device.startswith("nano"):
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
