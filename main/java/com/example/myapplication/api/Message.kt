// api/Message.kt
package com.example.myapplication.api

data class Message(
    val role: String, // "user" or "assistant"
    val content: String
)