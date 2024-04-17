# Technical Specification
## Smart Contracts

Smart contracts covered by this plugin are:

|  Network | Version | Smart Contract | Address |
|   ----   |   ---   |      ----      |   ---   |
| Lisk Sepolia Testnet     | -  | ERC1967Proxy  | `0x5c3a68B5C635Ce0DA7648C30A1B83A61C376bd87` |

## Functions

Following functions are covered by this plugins:

|Contract |    Function   | Selector  | Displayed Parameters |
|   ---   |    ---        | ---       | --- |
|ERC1967Proxy  | claimRegularAccount           | `0xf6de242d`| <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderPublicKey</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table>                                                                                               |
|ERC1967Proxy  | claimMultisigAccount           | `0x2f559f68`| <table> <tbody> <tr><td><code>uint256 claimAmount</code></td></tr> <tr><td><code>bytes senderAddress</code></td></tr> <tr><td><code>address recipientAddress</code></td></tr> </tbody> </table>                                                                                               |
