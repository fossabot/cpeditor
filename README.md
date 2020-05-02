# CP Editor
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-1-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

<img src=assets/icon.ico height="80" width="80">

CP Editor is a Qt-based, lightweight and cross-platform code editor specially designed for competitive programming.

It makes your competitive coding life easier :grin: by automating many things for you.

[Features](https://cpeditor.github.io/) | [Installation](doc/INSTALL.md) | [Usage](doc/MANUAL.md) | [Changelog](doc/CHANGELOG.md) | [Contributing](CONTRIBUTING.md) | [FAQ](#faq) | [中文](README_zh-CN.md) | [Русский](README_ru-RU.md)

---

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ce0f297f31f74485b0d340949d08d605)](https://www.codacy.com/gh/cpeditor/cpeditor)
[![GitHub All Releases](https://img.shields.io/github/downloads/cpeditor/cpeditor/total?label=downloads%40all)](https://github.com/cpeditor/cpeditor/releases)
[![Slack Workspace](https://img.shields.io/badge/join-slack-success)](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)
[![Telegram Group](https://img.shields.io/badge/join-telegram%20chat-success)](https://t.me/cpeditor)
[![Help wanted issues](https://img.shields.io/github/issues/cpeditor/cpeditor/help%20wanted)](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

|  type  |                           branch                           |                                                                                                       build                                                                                                        |                                                                   downloads                                                                    |                                                     AUR                                                      |
| :----: | :--------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------: |
| alpha  | [master](https://github.com/cpeditor/cpeditor/tree/master) | [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=master&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Amaster) |                                                                       -                                                                        | [![AUR-git](https://img.shields.io/badge/aur-master-blue)](https://aur.archlinux.org/packages/cpeditor-git/) |
|  beta  |   [v6.3](https://github.com/cpeditor/cpeditor/tree/v6.3)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.3&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.3)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.3.1/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.3.1) |                                                      -                                                       |
| stable |   [v6.2](https://github.com/cpeditor/cpeditor/tree/v6.2)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.2&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.2)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.2.4/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.2.4) | [![AUR version](https://img.shields.io/aur/version/cpeditor)](https://aur.archlinux.org/packages/cpeditor/)  |

## Get Started

- [Releases](https://github.com/cpeditor/cpeditor/releases)
- [Install in different ways](doc/INSTALL.md)
- [Get Started and Tips](doc/MANUAL.md)

## Contributing

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Contributing Guidelines](CONTRIBUTING.md)
- [Good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- [Help wanted issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

## Get help

1. Read the [manual](doc/MANUAL.md) and the [FAQ](#faq) first, also go through the menus and preferencecs to see if there are what you are looking for.
2. Ask Google first if it's not very editor-related. (e.g. How to install Clang Format? What does this compilation error mean?)
3. Search in the [Issues](https://github.com/cpeditor/cpeditor/issues) and make sure your bug report/feature request is not duplicated.
4. Please follow the issue template and provide detailed information, it will help us to give you better feedback.
5. If it's a feature request or bug report rather than a question, please open an issue instead of asking on Telegram or Slack so that it can be tracked easier.

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Ask on Telegram](https://t.me/cpeditor)
- [Ask on Slack](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)

## FAQ

- I am using Java and the editor can't run my codes.
   - You have to use a **non-public** class named **a** for your solution.
- I get **DLL Missing error** when launching the application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
- How to use whole-application dark theme?
   - Now you can use whole-application dark theme on KDE, macOS and Windows 10. You need to set the system theme to dark theme, then CP Editor will use dark theme, too. However, on Windows 10, if you use the dark theme, you may need to restart the application after changing the theme of the code editor.
- How to fetch testcases from the websites?
   - You have to install [Competitive Companion](https://github.com/jmerle/competitive-companion) on your browser, and use it on the website while CP Editor is running.
- How to submit to CF inside the editor?
   - You have to either parse the problem from Competitive Companion, or set the problem URL in the right-click menu of the tabs. Then you'll see the submit button.
- I am using it on Codeforces but the submit button is not clickable.
   - It's because the editor can't run the `cf` command. Please make sure it's in the PATH or set the path to it in the Preferences.
- When I click the Submit button, I get some message saying template is required?
   - `cf` tool requires you to configure it before you use it. Please run `cf config` to set the username & password and add a template.
- I got an invalid payload when parsing sample testcases?
   - Please try restarting the browser and the editor. It's known that there are few people always fail on this even after restarting, and Firefox is more stable than Google Chrome. We're really sorry if it always fails, you can change a browser as for now, we will try our best to investigate it.

License
----

[GNU General Public License Version 3.0](https://github.com/cpeditor/cpeditor/blob/master/LICENSE)

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://ouuan.github.io"><img src="https://avatars2.githubusercontent.com/u/30581822?v=4" width="100px;" alt=""/><br /><sub><b>Yufan You</b></sub></a><br /><a href="https://github.com/ouuan/cpeditor/commits?author=ouuan" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!