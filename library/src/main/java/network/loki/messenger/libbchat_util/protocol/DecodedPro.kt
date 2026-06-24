package network.loki.messenger.libbchat_util.protocol

import network.loki.messenger.libbchat_util.pro.ProProof
import network.loki.messenger.libbchat_util.pro.ProProofStatus

data class DecodedPro(
    val status: ProProofStatus,
    val proof: ProProof?,
    val proMessageFeatures: ProMessageFeatures,
    val proProfileFeatures: ProProfileFeatures,
)
