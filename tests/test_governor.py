from web3 import Web3
from .utils import run_test, load_contract

contract = load_contract(
    "8179793eF169b6EE7aAa2A5EE37d9463457ee757",
    "governor"
)

def test_governor_cast_vote(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("castVote", [
        86511774,
        1,
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_governor_cast_vote_with_reason(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("castVoteWithReason", [
        86511774,
        1,
        "Cast vote with reason"
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_governor_propose_one_element(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("propose", [
        [
            bytes.fromhex("77abc2dd8ca5021dbb8e91e63f574b1e440764d2"),
        ],
        [
            5000000000000000000,
        ],
        [
            bytes.fromhex("00"),
        ],
        "Test string"
    ])
    
    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_governor_propose_two_elements(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("propose", [
        [
            bytes.fromhex("77abc2dd8ca5021dbb8e91e63f574b1e440764d2"),
            bytes.fromhex("889bc2dd8ca5021dbb8e91e63f574b1e440764d2")
        ],
        [
            5000000000000000000,
            8000000000000000000
        ],
        [
            bytes.fromhex("00"),
        ],
        "Test string"
    ])
    
    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)