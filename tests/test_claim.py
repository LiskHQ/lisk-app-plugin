from web3 import Web3
from .utils import run_test, load_contract

contract = load_contract(
    "D7BE2Fd98BfD64c1dfCf6c013fC593eF09219994",
    "claim"
)

def test_claim_regular_account(backend, firmware, navigator, test_name, wallet_addr):
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

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_claim_multisig_account(backend, firmware, navigator, test_name, wallet_addr):
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

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)