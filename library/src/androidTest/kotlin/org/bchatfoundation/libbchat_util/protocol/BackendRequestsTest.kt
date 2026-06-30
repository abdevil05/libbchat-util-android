package org.bchatfoundation.libbchat_util.protocol

import androidx.test.ext.junit.runners.AndroidJUnit4
import org.bchatfoundation.libbchat_util.pro.BackendRequests
import org.junit.Assert.assertNotNull
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class BackendRequestsTest {

    @Test
    fun getProviderMetadataWorks() {
        val metadata = BackendRequests.getPaymentProviderMetadata(BackendRequests.PAYMENT_PROVIDER_GOOGLE_PLAY)
        assertNotNull(metadata)
    }
}