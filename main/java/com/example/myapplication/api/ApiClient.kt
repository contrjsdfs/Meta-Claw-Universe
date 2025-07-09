// api/ApiClient.kt
package com.example.myapplication.api

import com.google.gson.Gson
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import org.videolan.BuildConfig
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.Body
import retrofit2.http.Header
import retrofit2.http.POST
import java.util.concurrent.TimeUnit

object ApiClient {
    private const val BASE_URL = "https://api.deepseek.com/"
    const val API_KEY = "sk-29e062bda77b48b8acde5c54e927ebcf"

    private val loggingInterceptor = HttpLoggingInterceptor().apply {
        level = if (BuildConfig.DEBUG) {
            HttpLoggingInterceptor.Level.BODY
        } else {
            HttpLoggingInterceptor.Level.NONE
        }
    }

    private val okHttpClient = OkHttpClient.Builder()
        .connectTimeout(30, TimeUnit.SECONDS)
        .readTimeout(30, TimeUnit.SECONDS)
        .writeTimeout(30, TimeUnit.SECONDS)
        .addInterceptor(loggingInterceptor)
        .build()

    private val retrofit = Retrofit.Builder()
        .baseUrl(BASE_URL)
        .client(okHttpClient)
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    val deepSeekApiService: DeepSeekApiService by lazy {
        retrofit.create(DeepSeekApiService::class.java)
    }

    data class ChatRequest(
        val model: String = "deepseek-reasoner",
        val messages: List<Message>,
        val temperature: Double = 0.7,
        val max_tokens: Int = 3000,
        val stream: Boolean = true
    )

    data class StreamResponse(
        val choices: List<StreamChoice>
    )

    data class StreamChoice(
        val delta: MessageDelta,
        val finish_reason: String?,
        val index: Int
    )

    data class MessageDelta(
        val role: String?,
        val content: String?
    )
}

interface DeepSeekApiService {
    @POST("v1/chat/completions")
    suspend fun chatCompletions(
        @Header("Authorization") authHeader: String,
        @Body request: ApiClient.ChatRequest
    ): Response<okhttp3.ResponseBody>
}