package network.loki.messenger.libbchat_util.pro

import androidx.annotation.Keep
import network.loki.messenger.libbchat_util.util.Bytes

data class ProConfig(
    val proProof: ProProof,
    val rotatingPrivateKey: Bytes
) {
    @Keep
    constructor(
        proProof: ProProof,
        rotatingPrivateKey: ByteArray
    ) : this(
        proProof,
        Bytes(rotatingPrivateKey)
    )
}
