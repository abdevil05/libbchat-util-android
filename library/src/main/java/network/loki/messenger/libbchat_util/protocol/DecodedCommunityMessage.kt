package network.loki.messenger.libbchat_util.protocol

import androidx.annotation.Keep
import network.loki.messenger.libbchat_util.util.Bytes

data class DecodedCommunityMessage(
    val decodedPro: DecodedPro?,
    val contentPlainText: Bytes,
) {
    @Keep
    constructor(
        decodedPro: DecodedPro?,
        contentPlainText: ByteArray,
    ): this(
        decodedPro = decodedPro,
        contentPlainText = Bytes(contentPlainText),
    )
}