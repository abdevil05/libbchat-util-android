package network.loki.messenger.libbchat_util.util

import network.loki.messenger.libbchat_util.LibBChatUtilCApi

object Util : LibBChatUtilCApi() {
    private external fun lengthForCodepoints(str: String, maxCodepoints: Int): Int

    fun truncateCodepoints(str: String, maxCodepoints: Int): String =
        str.take(lengthForCodepoints(str, maxCodepoints))

    external fun countCodepoints(str: String): Int
}