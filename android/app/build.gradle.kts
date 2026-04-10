import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
    id("org.jetbrains.kotlin.plugin.compose")
}

android {
    namespace = "io.github.iudah.greygreg"
    compileSdk = 34
    ndkVersion = "28.2.13676358"

    defaultConfig {
        applicationId = "io.github.iudah.greygreg"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        externalNativeBuild {
            cmake {
                arguments +=
                        listOf(
                                "-DANDROID_ARM_NEON=TRUE",
                                "-DANDROID_TOOLCHAIN=clang",
                                "-DANDROID_PLATFORM=android-24"
                        )
                cFlags += listOf("-Wall", "-Wextra", "-Wpendatic")
                targets += listOf("greg")
            }
        }
        ndk {
            // abiFilters+=listOf( "x86", "x86_64", "armeabi", "armeabi-v7a", "arm64-v8a")
            abiFilters += listOf("armeabi-v7a")
        }
    }

    externalNativeBuild {
        cmake {
            path = file("../../CMakeLists.txt")
            version = "3.30.5"
        }
    }

    buildFeatures { compose = true }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
}

tasks.withType<org.jetbrains.kotlin.gradle.tasks.KotlinCompile>().configureEach {
    compilerOptions { jvmTarget.set(JvmTarget.JVM_17) }
}

dependencies {
    implementation(platform("androidx.compose:compose-bom:2024.08.00"))
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.material3:material3")
    implementation("androidx.activity:activity-compose:1.9.0")
    implementation("com.google.android.material:material:1.11.0")
}
