from web3 import Web3
from .utils import run_test, load_contract

contract = load_contract(
    "9fA3CA453EbfB7a6d1085153D83a2988eE822BD0",
    "airdrop"
)

def test_claim_airdrop(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("claimAirdrop", [
        bytes.fromhex("77abc2dd8ca5021dbb8e91e63f574b1e440764d2"),
        20000000000000000000,
        [
            bytes.fromhex("9ca797d905e78f38685b61f62521632ef486bc6ce3e707d2af41b7fe146303c1"),
        ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)