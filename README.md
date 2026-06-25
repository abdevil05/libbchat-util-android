### libbchat-util Android bridging code

This repository contains the bridging code allowing the Java/Kotlin client to access to the [libbchat-util](https://github.com/bchat-foundation/libbchat-util/) library.

#### Quick start

To start using this library, add this into your top level `build.gradle`'s repository settings

```gradle
maven {
    url uri("https://maven.pkg.github.com/abdevil05/libbchat-util-android")
    credentials {
        username = findProperty("gpr.user") ?: System.getenv("GITHUB_ACTOR")
        password = findProperty("gpr.key") ?: System.getenv("GITHUB_TOKEN")
    }
    content {
        includeGroup('org.bchatfoundation')
    }
}
```

Then in your app's `build.gradle', you can add this as dependency:

```gradle
implementation 'org.bchatfoundation:libbchat-util-android:LATEST_VERSION_NUMBER'
```

All tags pushed to this repository will be built and published to GitHub Packages. You can find the latest version by visiting https://github.com/abdevil05/libbchat-util-android/tags

GitHub Packages' Maven URL is a registry endpoint, not a browser page. After the first successful publish, view the package from the Packages tab on https://github.com/abdevil05 or fetch Maven metadata from the registry with GitHub credentials.

#### Versioning

This library's versioning rules are:
1. If there is a tag related to this commit, the tag will be used as the version name
2. Otherwise, the version name will look like "lastGitTag-numberOfChangesSinceLastTag-shortCommitHash", for example "1.0.0-5-abcd12345". "-dirty" will be appended if the git workspace contains uncommitted changes.

Normally you don't have to worry about versioning as it's fully automatic and deterministic. If
changes warrant a new release, then BChat devs should push a tag,
and we'll have a formal release version built.

#### Development & deployment

Everyone's welcome to open a Pull Request. Release artifacts are published by GitHub Actions to GitHub Packages on pushes to `main`/`master` and version tags.
