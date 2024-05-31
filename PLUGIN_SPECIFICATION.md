# Technical Specification

## Smart Contracts

Smart contracts covered by this plugin are:

| Network              | Version | Smart Contract | Address                                          |
| -------------------- | ------- | -------------- | ------------------------------------------------ |
| Lisk Sepolia Testnet | -       | TokenClaim     | `0x3D4190b08E3E30183f5AdE3A116f2534Ee3a4f94`     |
| Lisk Sepolia Testnet | -       | Reward         | `0xFd322B4724C497E59D48fff8f79c16b4D48837f5`     |
| Lisk Sepolia Testnet | -       | Airdrop        | `0x9fA3CA453EbfB7a6d1085153D83a2988eE822BD0`     |
| Lisk Sepolia Testnet | -       | Governor       | `0xf9181aaD773d423A2cc0155Cb4263E563D51B467`     |
| Lisk Mainnet         | -       | TokenClaim     | `0xD7BE2Fd98BfD64c1dfCf6c013fC593eF09219994`     |
| Lisk Mainnet         | -       | Reward         | `0xD35ca9577a9DADa7624a35EC10C2F55031f0Ab1f`     |
| Lisk Mainnet         | -       | Airdrop        | `TODO`                                           |
| Lisk Mainnet         | -       | Governor       | `0x58a61b1807a7bDA541855DaAEAEe89b1DDA48568TODO` |


## Functions

Following functions are covered by this plugins:

| Contract   | Function                 | Selector     | Displayed Parameters                                                                                                                                                                              |
| ---------- | ------------------------ | ------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| TokenClaim | claimRegularAccount      | `0xf6de242d` | <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderPublicKey</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table> |
| TokenClaim | claimMultisigAccount     | `0x2f559f68` | <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderAddress</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table>   |
| Reward     | createPosition           | `0xd1aaef05` | <table> <tbody> <tr><td><code>uint256 lockAmount</code></td></tr> <tr><td><code>uint256 lockDuration</code></td></tr> </tbody> </table>                                                           |
| Reward     | initiateFastUnlock       | `0x864c8725` | <table> <tbody> <tr><td><code>uint256[] lockIDs</code></td></tr> </tbody> </table>                                                                                                                |
| Reward     | claimRewards             | `0x5eac6239` | <table> <tbody> <tr><td><code>uint256[] lockIDs</code></td></tr> </tbody> </table>                                                                                                                |
| Reward     | pauseUnlocking           | `0xfe042b5b` | <table> <tbody> <tr><td><code>uint256[] lockIDs</code></td></tr> </tbody> </table>                                                                                                                |
| Reward     | resumeUnlockingCountdown | `0x82d4ae58` | <table> <tbody> <tr><td><code>uint256[] lockIDs</code></td></tr> </tbody> </table>                                                                                                                |
| Reward     | increaseLockingAmount    | `0xf94415ca` | <table> <tbody> <tr><td><code>(uint256 lockID, uint256 amountIncrease)[]</code></td></tr> </tbody> </table>                                                                                       |
| Reward     | extendDuration           | `0x2d412a71` | <table> <tbody> <tr><td><code>(uint256 lockID, uint256 durationExtension)[]</code></td></tr> </tbody> </table>                                                                                    |
| Reward     | deletePositions          | `0x221b2b41` | <table> <tbody> <tr><td><code>uint256[] lockIDs</code></td></tr> </tbody> </table>                                                                                                                |
| Reward     | addUnusedRewards         | `0x315d4222` | <table> <tbody> <tr><td><code>uint256 amount</code></td></tr> <tr><td><code>uint256 duration</code></td></tr> <tr><td><code>uint256 delay</code></td></tr> </tbody> </table>                      |
| Reward     | fundStakingRewards       | `0xebcb3818` | <table> <tbody> <tr><td><code>uint256 amount</code></td></tr> <tr><td><code>uint256 duration</code></td></tr> <tr><td><code>uint256 delay</code></td></tr> </tbody> </table>                      |
| Airdrop    | claimAirdrop             | `0x0c1646b7` | <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes liskAddress</code></td></tr> </tbody> </table>                                                             |
| Governor   | propose                  | `0x7d5e81e2` | <table> <tbody> <tr><td><code>address[] targets</code></td></tr> <tr><td><code>uint256[] values</code></td></tr> </tbody> </table>                                                                |
| Governor   | castVote                 | `0x56781388` | <table> <tbody> <tr><td><code>uint256 proposalId</code></td></tr> <tr><td><code>uint256 support</code></td></tr> </tbody> </table>                                                                |
| Governor   | castVoteWithReason       | `0x7b3c71d3` | <table> <tbody> <tr><td><code>uint256 proposalId</code></td></tr> <tr><td><code>uint256 support</code></td></tr> <tr><td><code>string reason</code></td></tr> </tbody> </table>                   |