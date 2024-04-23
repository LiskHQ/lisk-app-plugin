# Technical Specification
## Smart Contracts

Smart contracts covered by this plugin are:

|  Network | Version | Smart Contract | Address |
|   ----   |   ---   |      ----      |   ---   |
| Lisk Sepolia Testnet     | -  | TokenClaim  | `0x4bf6F67f7B1EF8C7a6C9C7a202C8b6838a3490e4` |
| Lisk Sepolia Testnet     | -  | Reward  | `0x0xfB072D568840BC67846B24d64D66095C1f932F99` |
## Functions

Following functions are covered by this plugins:

|Contract |    Function   | Selector  | Displayed Parameters |
|   ---   |    ---        | ---       | --- |
|TokenClaim  | claimRegularAccount           | `0xf6de242d`| <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderPublicKey</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table>                                                                                               |
|TokenClaim  | claimMultisigAccount           | `0x2f559f68`| <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderAddress</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table>                                                                                               |
|Reward  | createPosition           | `0xd1aaef05`| <table> <tbody> <tr><td><code>uint256 lockAmount</code></td></tr> <tr><td><code>uint256 lockDuration</code></td></tr> </tbody> </table>                                                                                               |
