import org.jetbrains.kotlin.gradle.dsl.JvmTarget

plugins {
    id("com.android.library")
    id("org.jetbrains.kotlin.android")
    id("org.jetbrains.kotlin.plugin.serialization")
    id("maven-publish")
    id("com.google.protobuf") version "0.9.5"
}

group = "org.bchatfoundation"
version = System.getenv("VERSION") ?: "dev-snapshot"

val protobufVersion = "4.34.0"

android {
    namespace = "org.bchatfoundation.libbchat_util"
    compileSdk = 36
    ndkVersion = "27.0.12077973"

    defaultConfig {
        minSdk = 26

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        externalNativeBuild {
            cmake {
                arguments += listOf(
                    "-DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON",
                    "-DCMAKE_POLICY_VERSION_MINIMUM=3.5",
                    "-DENABLE_NETWORKING=OFF",
                    "-DENABLE_NETWORKING_SROUTER=OFF",
                    "-DSUBMODULE_CHECK=OFF",
                )
                targets("bchat_util")
                abiFilters += listOf("arm64-v8a", "armeabi-v7a")
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1+"
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            consumerProguardFile(file("proguard-rules.pro"))

            externalNativeBuild {
                cmake {
                    arguments += listOf("-DCMAKE_BUILD_TYPE=Release")
                }
            }
        }

        debug {
            packaging {
                jniLibs {
                    keepDebugSymbols += "**/*.so"
                }
            }
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }
}

kotlin {
    compilerOptions {
        jvmTarget = JvmTarget.JVM_11
    }
}

protobuf {
    protoc {
        artifact = "com.google.protobuf:protoc:$protobufVersion"
    }

    plugins {
        generateProtoTasks {
            all().forEach { task ->
                task.builtins {
                    create("java") {}
                }
            }
        }
    }
}

publishing {
    publications {
        create<MavenPublication>("release") {
            groupId = project.group.toString()
            artifactId = project.name
            version = project.version.toString()

            pom {
                url = "https://getbchat.io"

                licenses {
                    license {
                        name = "GNU GENERAL PUBLIC LICENSE, Version 3"
                        url = "https://www.gnu.org/licenses/gpl-3.0.txt"
                    }
                }

                scm {
                    connection = "scm:git:https://github.com/abdevil05/libbchat-util-android"
                    url = "https://github.com/abdevil05/libbchat-util-android"
                }
            }

            afterEvaluate {
                from(components["release"])
            }
        }
    }

    repositories {
        maven {
            name = "local"
            url = uri(layout.buildDirectory.dir("repo"))
        }
        maven {
            name = "GitHubPackages"
            url = uri("https://maven.pkg.github.com/abdevil05/libbchat-util-android")
            credentials {
                username = System.getenv("GITHUB_ACTOR") ?: ""
                password = System.getenv("GITHUB_TOKEN") ?: ""
            }
        }
    }
}

dependencies {

    androidTestImplementation("junit:junit:4.13.2")
    androidTestImplementation("androidx.test:runner:1.7.0")
    androidTestImplementation("androidx.test:rules:1.7.0")
    androidTestImplementation("androidx.test.ext:junit:1.3.0")

    implementation("androidx.annotation:annotation:1.9.1")
    implementation("org.jetbrains.kotlinx:kotlinx-serialization-core:1.10.0")

    api("com.google.protobuf:protobuf-java:$protobufVersion")

    protobuf(files("../libbchat-util/proto/BChatProtos.proto"))
}
