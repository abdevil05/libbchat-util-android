package org.bchatfoundation.libbchat_util.protocol

import org.bchatfoundation.libbchat_util.pro.ProProof
import org.bchatfoundation.libbchat_util.pro.ProProofStatus

data class DecodedPro(
    val status: ProProofStatus,
    val proof: ProProof?,
    val proMessageFeatures: ProMessageFeatures,
    val proProfileFeatures: ProProfileFeatures,
)
