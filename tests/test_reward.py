from web3 import Web3
from .utils import run_test, load_contract

contract = load_contract(
    "D35ca9577a9DADa7624a35EC10C2F55031f0Ab1f",
    "reward"
)

def test_create_position(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("createPosition", [
            86511774,
            100
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_initiate_fast_unlock(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("initiateFastUnlock", [
            [
                86511774,
                100,
                34532,
                34,
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_claim_rewards(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("claimRewards", [
            [
                86511774,
                100,
                34532,
                34,
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_pause_unlocking(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("pauseUnlocking", [
            [
                86511774,
                100,
                34532,
                34,
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_resume_unlocking(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("resumeUnlockingCountdown", [
            [
                86511774,
                100,
                34532,
                34,
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_increase_locking_amount(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("increaseLockingAmount", [
            [
                (2024, 2000000023003000000),
                (9998, 1000000000000000000),
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_extend_duration(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("extendDuration", [
            [
                (2024, 6),
                (9998, 8),
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_delete_positions(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("deletePositions", [
            [
                42344,
                8643,
                777777777,
                666,
            ]
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_add_unused_rewards(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("addUnusedRewards", [
            20000000000000000000,
            12,
            2
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)

def test_fund_staking_rewards(backend, firmware, navigator, test_name, wallet_addr):
    data = contract.encodeABI("fundStakingRewards", [
            300000000000000000000,
            7,
            6
    ])

    run_test(contract, data, backend, firmware, navigator, test_name, wallet_addr)