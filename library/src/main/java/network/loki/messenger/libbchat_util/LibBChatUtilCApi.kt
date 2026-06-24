package network.loki.messenger.libbchat_util

/**
 * Base class for all objects that need access to the native library.
 *
 * This is needed so that the native library can be loaded as soon as any object is accessed.
 */
abstract class LibBChatUtilCApi {
    init {
        System.loadLibrary("bchat_util")
    }
}