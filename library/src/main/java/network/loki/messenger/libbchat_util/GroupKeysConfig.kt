package network.loki.messenger.libbchat_util

class GroupKeysConfig private constructor(
    pointer: Long,
    private val info: GroupInfoConfig,
    private val members: GroupMembersConfig
): ConfigSig(pointer), MutableGroupKeysConfig {
    companion object {
        private external fun newInstance(
            userSecretKey: ByteArray,
            groupPublicKey: ByteArray,
            groupSecretKey: ByteArray? = null,
            initialDump: ByteArray?,
            infoPtr: Long,
            members: Long
        ): Long
    }

    constructor(
        userSecretKey: ByteArray,
        groupPublicKey: ByteArray,
        groupAdminKey: ByteArray?,
        initialDump: ByteArray?,
        info: GroupInfoConfig,
        members: GroupMembersConfig
    ) : this(
        newInstance(
            userSecretKey,
            groupPublicKey,
            groupAdminKey,
            initialDump,
            info.pointer,
            members.pointer
        ),
        info,
        members
    )

    override fun namespace() = Namespace.GROUP_KEYS()

    external override fun groupKeys(): List<ByteArray>
    external override fun groupEncKey(): ByteArray
    external override fun needsDump(): Boolean
    external override fun dump(): ByteArray
    external fun loadKey(message: ByteArray,
                         hash: String,
                         timestampMs: Long,
                         infoPtr: Long,
                         membersPtr: Long): Boolean

    override fun loadKey(message: ByteArray, hash: String, timestampMs: Long): Boolean {
        return loadKey(message, hash, timestampMs, info.pointer, members.pointer)
    }

    override fun loadAdminKey(adminKey: ByteArray) {
        loadAdminKey(adminKey, info.pointer, members.pointer)
    }

    private external fun loadAdminKey(adminKey: ByteArray, infoPtr: Long, membersPtr: Long)

    external override fun needsRekey(): Boolean
    external override fun pendingKey(): ByteArray?
    private external fun supplementFor(userBChatIds: Array<String>): ByteArray
    override fun supplementFor(userBChatIds: List<String>): ByteArray {
        return supplementFor(userBChatIds.toTypedArray())
    }

    external override fun pendingConfig(): ByteArray?
    external override fun activeHashes(): List<String>
    external fun rekey(infoPtr: Long, membersPtr: Long): ByteArray

    external override fun encrypt(plaintext: ByteArray): ByteArray
    external override fun decrypt(ciphertext: ByteArray): Pair<ByteArray, String>?

    external override fun keys(): List<ByteArray>

    external override fun makeSubAccount(bchatId: String, canWrite: Boolean, canDelete: Boolean): ByteArray
    external override fun getSubAccountToken(bchatId: String, canWrite: Boolean, canDelete: Boolean): ByteArray

    external override fun subAccountSign(message: ByteArray, signingValue: ByteArray): SwarmAuth

    external override fun currentGeneration(): Int
    external fun admin(): Boolean
    external override fun size(): Int

    data class SwarmAuth(
        val subAccount: String,
        val subAccountSig: String,
        val signature: String
    )
}