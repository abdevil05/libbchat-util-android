package org.bchatfoundation.libbchat_util.util

data class BaseCommunityInfo(val baseUrl: String, val room: String, val pubKeyHex: String) {
    companion object {
        init {
            System.loadLibrary("bchat_util")
        }
        external fun parseFullUrl(fullUrl: String): Triple<String, String, ByteArray>?
    }

    external fun fullUrl(): String
}