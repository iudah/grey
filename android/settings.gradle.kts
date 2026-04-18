pluginManagement {
    repositories {
        // maven {
        //      url = uri("/data/data/com.termux/files/home/storage/shared/Jay/app-dev/downloads") 
        // }
        gradlePluginPortal()
        google()
        mavenCentral()
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        // maven {
        //      url = uri("/data/data/com.termux/files/home/storage/shared/Jay/app-dev/downloads") 
        // }
        google()
        mavenCentral()
    }
}

rootProject.name = "grey"
include(":app")
