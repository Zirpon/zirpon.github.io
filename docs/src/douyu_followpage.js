// ==UserScript==
// @name         Douyu斗鱼 主播开播下播提醒 + 粤语/国语语音播报通知
// @namespace    http://tampermonkey.net/
// @version      3.3.4
// @description  手动打开关注页面并放置在后台(https://www.douyu.com/directory/myFollow)  有主播开播/更改标题时自动发送通知提醒
// @author       anonymous, hlc1209, P
// @icon         https://www.douyu.com/favicon.ico
// @match        https://www.douyu.com/directory/myFollow
// @grant GM_xmlhttpRequest
// @grant GM_openInTab
// @grant GM_notification
// @grant GM_registerMenuCommand
// @grant GM_setValue
// @grant GM_getValue
// @license MIT
// @downloadURL https://update.greasyfork.org/scripts/498616/Douyu%E6%96%97%E9%B1%BC%20%E4%B8%BB%E6%92%AD%E5%BC%80%E6%92%AD%E4%B8%8B%E6%92%AD%E6%8F%90%E9%86%92%20%2B%20%E7%B2%A4%E8%AF%AD%E5%9B%BD%E8%AF%AD%E8%AF%AD%E9%9F%B3%E6%92%AD%E6%8A%A5%E9%80%9A%E7%9F%A5.user.js
// @updateURL https://update.greasyfork.org/scripts/498616/Douyu%E6%96%97%E9%B1%BC%20%E4%B8%BB%E6%92%AD%E5%BC%80%E6%92%AD%E4%B8%8B%E6%92%AD%E6%8F%90%E9%86%92%20%2B%20%E7%B2%A4%E8%AF%AD%E5%9B%BD%E8%AF%AD%E8%AF%AD%E9%9F%B3%E6%92%AD%E6%8A%A5%E9%80%9A%E7%9F%A5.meta.js
// ==/UserScript==

var baseURL = "https://douyu.com";
var save = {};
var save_name = {};
shim_GM_notification();
/*--- Cross-browser Shim code follows:
Source: https://stackoverflow.com/questions/36779883/userscript-notifications-work-on-chrome-but-not-firefox
*/

/*--create style--*/
GM_getValue("LANG", "zh-CN");
GM_getValue("RATE", 1);
GM_getValue("switchVoice", true);
GM_getValue("GM_notice", true);

var domHead = document.getElementsByTagName("head")[0];

var domStyle = document.createElement("style");

domStyle.type = "text/css";

domStyle.rel = "stylesheet";

class BaseClass {
    constructor() {
        GM_registerMenuCommand("设置", () => this.menuSet());
        this.setStyle();
    }

    setStyle() {
        let menuSetStyle = `
            .zhmMask{
                z-index:999999999;
                background-color:#000;
                position: fixed;top: 0;right: 0;bottom: 0;left: 0;
                opacity:0.8;
            }
            .wrap-box{
                z-index:1000000000;
                position:fixed;;top: 50%;left: 50%;transform: translate(-50%, -200px);
                width: 300px;
                color: #555;
                background-color: #fff;
                border-radius: 5px;
                overflow:hidden;
                font:16px numFont,PingFangSC-Regular,Tahoma,Microsoft Yahei,sans-serif !important;
                font-weight:400 !important;
            }
            .setWrapHead{
                background-color:#f24443;height:40px;color:#fff;text-align:center;line-height:40px;
            }
            .setWrapLi{
                margin:0px;padding:0px;
            }
            .setWrapLi li{
                background-color: #fff;
                border-bottom:1px solid #eee;
                margin:0px !important;
                padding:12px 20px;
                display: flex;
                justify-content: space-between;align-items: center;
                list-style: none;
            }

            .setWrapLiContent{
                display: flex;justify-content: space-between;align-items: center;
            }
            .setWrapSave{
                position:absolute;top:-2px;right:10px;font-size:24px;cursor:pointer
            }
            .iconSetFoot{
                position:absolute;bottom:0px;padding:10px 20px;width:100%;
            z-index:1000000009;background:#fef9ef;
            }
            .iconSetFootLi{
                margin:0px;padding:0px;
                font-size: 12px;
            }

            .iconSetFootLi li{
                display: inline-flex;
                padding:0px 2px;
                justify-content: space-between;align-items: center;
                font-size: 12px;
            }
            .iconSetFootLi li a{
                color:#555;
            }
            .iconSetFootLi a:hover {
                color:#fe6d73;
            }
            .iconSetPage{
                z-index:1000000001;
                position:absolute;top:0px;left:300px;
                background:#fff;
                width:300px;
                height:100%;
            }
            .iconSetUlHead{
            padding:0px;
            margin:0px;
            }
            .iconSetPageHead{
                border-bottom:1px solid #ccc;
                height:40px;
                line-height:40px;
                display: flex;
                justify-content: space-between;
                align-items: center;
                background-color:#fe6d73;
                color:#fff;
                font-size: 15px;
            }
            .iconSetPageLi{
                margin:0px;padding:0px;
            }
            .iconSetPageLi li{
                list-style: none;
                padding:8px 20px;
                border-bottom:1px solid #eee;
            }
            .zhihuSetPage{
                z-index:1000000002;position:absolute;top:0px;left:300px;background:#fff;width:300px;height:100%;
            }
            .iconSetPageInput{
                display: flex !important;justify-content: space-between;align-items: center;
            }
            .zhihuSetPageLi{
                margin:0px;padding:0px;
                height:258px;
                overflow-y: scroll;
            }

            .zhihuSetPageContent{
                display: flex !important;justify-content: space-between;align-items: center;
            }

            .zhm_circular{
                width: 40px;height: 20px;border-radius: 16px;transition: .3s;cursor: pointer;box-shadow: 0 0 3px #999 inset;
            }
            .round-button{
                width: 20px;height: 20px;;border-radius: 50%;box-shadow: 0 1px 5px rgba(0,0,0,.5);transition: .3s;position: relative;
            }
            .zhm_back{
                border: solid #FFF; border-width: 0 3px 3px 0; display: inline-block; padding: 3px;transform: rotate(135deg);  -webkit-transform: rotate(135deg);margin-left:10px;cursor:pointer;
            }
            .to-right{
                margin-left:20px; display: inline-block; padding: 3px;transform: rotate(-45deg); -webkit-transform: rotate(-45deg);cursor:pointer;

            }
            .iconSetSave{
                font-size:24px;cursor:pointer;margin-right:5px;margin-bottom:4px;color:#FFF;
            }
            .zhm_set_page{
                z-index:1000000003;
                position:absolute;
                top:0px;left:300px;
                background:#fff;
                width:300px;
                height:100%;
            }
            .zhm_set_page_header{
                border-bottom:1px solid #ccc;
                height:40px;
                line-height:40px;
                display: flex;
                justify-content: space-between;
                align-items: center;
                background-color:#fe6d73;
                color:#fff;
                font-size: 15px;
            }
            .zhm_set_page_content{
                display: flex !important;justify-content: space-between;align-items: center;
            }
            .zhm_set_page_list{
                margin:0px;padding:0px;
                height: 220px;
                overflow-y: scroll;
            }

            .zhm_set_page_list::-webkit-scrollbar {
                /*滚动条整体样式*/
                width : 0px;  /*高宽分别对应横竖滚动条的尺寸*/
                height: 1px;
            }
            .zhm_set_page_list::-webkit-scrollbar-thumb {
                /*滚动条里面小方块*/
                border-radius   : 2px;
                background-color: #fe6d73;
            }
            .zhm_set_page_list::-webkit-scrollbar-track {
                /*滚动条里面轨道*/
                box-shadow   : inset 0 0 5px rgba(0, 0, 0, 0.2);
                background   : #ededed;
                border-radius: 10px;
            }
            .zhm_set_page_list li{
                /*border-bottom:1px solid #ccc;*/
                padding:12px 20px;
                display:block;
                border-bottom:1px solid #eee;
            }
            li:last-child{
                border-bottom:none;
            }
            .zhm_scroll{
            overflow-y: scroll !important;
            }
            .zhm_scroll::-webkit-scrollbar {
                /*滚动条整体样式*/
                width : 0px;  /*高宽分别对应横竖滚动条的尺寸*/
                height: 1px;
            }
            .zhm_scroll::-webkit-scrollbar-thumb {
                /*滚动条里面小方块*/
                border-radius   : 2px;
                background-color: #fe6d73;
            }
            .zhm_scroll::-webkit-scrollbar-track {
                /*滚动条里面轨道*/
                box-shadow   : inset 0 0 5px rgba(0, 0, 0, 0.2);
                background   : #ededed;
                border-radius: 10px;
            }
            /*-form-*/
            :root {
                --base-color: #434a56;
                --white-color-primary: #f7f8f8;
                --white-color-secondary: #fefefe;
                --gray-color-primary: #c2c2c2;
                --gray-color-secondary: #c2c2c2;
                --gray-color-tertiary: #676f79;
                --active-color: #227c9d;
                --valid-color: #c2c2c2;
                --invalid-color: #f72f47;
                --invalid-icon: url("data:image/svg+xml;charset=utf8,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%2224%22%20height%3D%2224%22%20viewBox%3D%220%200%2024%2024%22%3E%20%3Cpath%20d%3D%22M13.41%2012l4.3-4.29a1%201%200%201%200-1.42-1.42L12%2010.59l-4.29-4.3a1%201%200%200%200-1.42%201.42l4.3%204.29-4.3%204.29a1%201%200%200%200%200%201.42%201%201%200%200%200%201.42%200l4.29-4.3%204.29%204.3a1%201%200%200%200%201.42%200%201%201%200%200%200%200-1.42z%22%20fill%3D%22%23f72f47%22%20%2F%3E%3C%2Fsvg%3E");
            }
            .text-input {
                font-size: 16px;
                position: relative;
                right:0px;
                z-index: 0;
            }
            .text-input__body {
                -webkit-appearance: none;
                -moz-appearance: none;
                appearance: none;
                background-color: transparent;
                border: 1px solid var(--gray-color-primary);
                border-radius: 3px;
                height: 1.7em;
                line-height: 1.7;
                overflow: hidden;
                padding: 2px 1em;
                text-overflow: ellipsis;
                transition: background-color 0.3s;
                width:55%;
                font-size:14px;
                box-sizing: initial;
            }
            .text-input__body:-ms-input-placeholder {
                color: var(--gray-color-secondary);
            }
            .text-input__body::-moz-placeholder {
                color: var(--gray-color-secondary);
            }
            .text-input__body::placeholder {
                color: var(--gray-color-secondary);
            }

            .text-input__body[data-is-valid] {
                padding-right: 1em;

            }
            .text-input__body[data-is-valid=true] {
                border-color: var(--valid-color);
            }
            .text-input__body[data-is-valid=false] {
                border-color: var(--invalid-color);
                box-shadow: inset 0 0 0 1px var(--invalid-color);
            }
            .text-input__body:focus {
                border-color: var(--active-color);
                box-shadow: inset 0 0 0 1px var(--active-color);
                outline: none;
            }
            .text-input__body:-webkit-autofill {
                transition-delay: 9999s;
                -webkit-transition-property: background-color;
                transition-property: background-color;
            }
            .text-input__validator {
                background-position: right 0.5em center;
                background-repeat: no-repeat;
                background-size: 1.5em;
                display: inline-block;
                height: 100%;
                left: 0;
                position: absolute;
                top: 0;
                width: 100%;
                z-index: -1;
            }
            .text-input__body[data-is-valid=false] + .text-input__validator {
                background-image: var(--invalid-icon);
            }
            .select-box {
                box-sizing: inherit;
                font-size: 16px;
                position: relative;
                transition: background-color 0.5s ease-out;
                width:90px;
            }
            .select-box::after {
                border-color: var(--gray-color-secondary) transparent transparent transparent;
                border-style: solid;
                border-width: 6px 4px 0;
                bottom: 0;
                content: "";
                display: inline-block;
                height: 0;
                margin: auto 0;
                pointer-events: none;
                position: absolute;
                right: -72px;
                top: 0;
                width: 0;
                z-index: 1;
            }
            .select-box__body {
                box-sizing: initial;
                -webkit-appearance: none;
                -moz-appearance: none;
                appearance: none;
                background-color: transparent;
                border: 1px solid var(--gray-color-primary);
                border-radius: 3px;
                cursor: pointer;
                height: 1.7em;
                line-height: 1.7;
                padding-left: 1em;
                padding-right: calc(1em + 16px);
                width: 140%;
                font-size:14px;
                padding-top:2px;
                padding-bottom:2px;
            }
            .select-box__body[data-is-valid=true] {
                border-color: var(--valid-color);
                box-shadow: inset 0 0 0 1px var(--valid-color);
            }
            .select-box__body[data-is-valid=false] {
                border-color: var(--invalid-color);
                box-shadow: inset 0 0 0 1px var(--invalid-color);
            }
            .select-box__body.focus-visible {
                border-color: var(--active-color);
                box-shadow: inset 0 0 0 1px var(--active-color);
                outline: none;
            }
            .select-box__body:-webkit-autofill {
                transition-delay: 9999s;
                -webkit-transition-property: background-color;
                transition-property: background-color;
            }
            .textarea__body {
                -webkit-appearance: none;
                -moz-appearance: none;
                appearance: none;
                background-color: transparent;
                border: 1px solid var(--gray-color-primary);
                border-radius: 0;
                box-sizing: initial;
                font: inherit;
                left: 0;
                letter-spacing: inherit;
                overflow: hidden;
                padding: 1em;
                position: absolute;
                resize: none;
                top: 0;
                transition: background-color 0.5s ease-out;
                width: 100%;
                }
            .textarea__body:only-child {
                position: relative;
                resize: vertical;
            }
            .textarea__body:focus {
                border-color: var(--active-color);
                box-shadow: inset 0 0 0 1px var(--active-color);
                outline: none;
            }
            .textarea__body[data-is-valid=true] {
                border-color: var(--valid-color);
                box-shadow: inset 0 0 0 1px var(--valid-color);
            }
            .textarea__body[data-is-valid=false] {
                border-color: var(--invalid-color);
                box-shadow: inset 0 0 0 1px var(--invalid-color);
            }

            .textarea ._dummy-box {
                border: 1px solid;
                box-sizing: border-box;
                min-height: 240px;
                overflow: hidden;
                overflow-wrap: break-word;
                padding: 1em;
                visibility: hidden;
                white-space: pre-wrap;
                word-wrap: break-word;
            }
            .toLeftMove{
                nimation:moveToLeft 0.5s infinite;
                -webkit-animation:moveToLeft 0.5s infinite; /*Safari and Chrome*/
                animation-iteration-count:1;
                animation-fill-mode: forwards;
            }

            @keyframes moveToLeft{
                from {left:200px;}
                to {left:0px;}
            }

            @-webkit-keyframes moveToLeft /*Safari and Chrome*/{
                from {left:200px;}
                to {left:0px;}
            }

            .toRightMove{
                nimation:moveToRight 2s infinite;
                -webkit-animation:moveToRight 2s infinite; /*Safari and Chrome*/
                animation-iteration-count:1;
                animation-fill-mode: forwards;
            }
            @keyframes moveToRight{
                from {left:0px;}
                to {left:2000px;}
            }

            @-webkit-keyframes moveToRight /*Safari and Chrome*/{
                from {left:0px;}
                to {left:200px;}
            }
            label {
            margin-right: 15px;
            line-height: 32px;
            }

            input {
            appearance: none;

            border-radius: 50%;
            width: 16px;
            height: 16px;

            border: 2px solid #999;
            transition: 0.2s all linear;
            margin-right: 5px;

            position: relative;
            top: 4px;
            }

            input:checked {
            border: 6px solid black;
            }

            button,
            legend {
            color: white;
            background-color: black;
            padding: 5px 10px;
            border-radius: 0;
            border: 0;
            font-size: 14px;
            }

            button:hover,
            button:focus {
            color: #999;
            }

            button:active {
            background-color: white;
            color: black;
            outline: 1px solid black;
            }
			/* 创建一个开关按钮的样式 */
			.switch {
				position: relative;
				display: inline-block;
				width: 60px;
				height: 34px;
			}

			/* 隐藏开关按钮的输入框 */
			.switch input {
				opacity: 0;
				width: 0;
				height: 0;
			}

			/* 创建滑块的样式 */
			.slider {
				position: absolute;
				cursor: pointer;
				top: 0;
				left: 0;
				right: 0;
				bottom: 0;
				background-color: #ccc;
				transition: 0.4s;
			}

			/* 创建滑块的初始状态 */
			.slider:before {
				position: absolute;
				content: "";
				height: 26px;
				width: 26px;
				left: 4px;
				bottom: 4px;
				background-color: white;
				transition: 0.4s;
			}
            /* 当输入框被选中时，改变滑块的背景颜色 */
            input:checked+.slider {
              background-color: #2196f3;
            }
        
            /* 当输入框被聚焦时，给滑块添加阴影 */
            input:focus+.slider {
              box-shadow: 0 0 1px #2196f3;
            }
        
            /* 当输入框被选中时，移动滑块 */
            input:checked+.slider:before {
              transform: translateX(26px);
            }
        
            /* 创建滑块的圆角样式 */
            .slider.round {
              border-radius: 34px;
            }
        
            /* 创建滑块的圆形样式 */
            .slider.round:before {
              border-radius: 50%;
            }            
            `;

        domStyle.appendChild(document.createTextNode(menuSetStyle));

        domHead.appendChild(domStyle);
    }

    menuSet() {
        let setHtml = "<div class='zhmMask'></div>";

        setHtml += "<div class='wrap-box' id='setWrap'>";

        setHtml +=
            "<ul class='iconSetUlHead'><li class='iconSetPageHead'><span></span><span>" +
            "语音播报设置" +
            "</span><span class='iconSetSave'>×</span></li></ul>";

        setHtml += "<ul class='setWrapLi'>";

        setHtml += "<br>";
        setHtml += "<div class='setWrapLiContent'>";
        setHtml += "<p>语音开关：</p>";
        setHtml += "<div>";
        setHtml += '\
            <label class="switch">\
                <input type="checkbox" id="togBtn" />\
                <div class="slider round"></div>\
            </label>'
        setHtml += "</div>";
        setHtml += "</div>";
        setHtml += "<br>";

        setHtml += "<br>";
        setHtml += "<div class='setWrapLiContent'>";
        setHtml += "<p>语种：</p>";
        setHtml += "<div>";
        setHtml +=
            "<input type='radio' name='lang' value='zh-CN' id='ch_sim'><label for='ch_sim'>国语</label>";
        setHtml += "</div>";
        setHtml += "<div>";
        setHtml +=
            "<input type='radio' name='lang' value='zh-HK' id='ch_sim'><label for='ch_sim'>粤语</label>";
        setHtml += "</div>";
        setHtml += "</div>";
        setHtml += "<br>";

        let setrate = GM_getValue("RATE", 1);
        setHtml += "<div class='setWrapLiContent'>";
        setHtml += "<p>语速：</p>";
        setHtml += '<select id="framework">';
        setHtml += '<option value="' + setrate + '">' + setrate + "</option>";
        setHtml += '<option value="0.5">0.5</option>';
        setHtml += '<option value="0.8">0.8</option>';
        setHtml += '<option value="1">1</option>';
        setHtml += '<option value="1.2">1.2</option>';
        setHtml += '<option value="1.5">1.5</option>';
        setHtml += '<option value="1.8">1.8</option>';
        setHtml += '<option value="2">2</option>';
        setHtml += '<option value="3">3</option>';
        setHtml += '<option value="4">4</option>';
        setHtml += '<option value="5">5</option>';
        setHtml += '<option value="6">6</option>';
        setHtml += '<option value="7">7</option>';
        setHtml += '<option value="8">8</option>';
        setHtml += '<option value="9">9</option>';
        setHtml += '<option value="10">10</option>';
        setHtml += "</select>";
        setHtml += '<button id="btn2">Submit</button>';
        setHtml += "</div>";

        setHtml += "<br>";
        setHtml += "<div class='setWrapLiContent'>";
        setHtml += "<p>通知弹窗开关：</p>";
        setHtml += "<div>";
        setHtml += '\
            <label class="switch">\
                <input type="checkbox" id="gmTogBtn" />\
                <div class="slider round"></div>\
            </label>'
        setHtml += "</div>";
        setHtml += "</div>";
        setHtml += "<br>";

        setHtml += "</ul>";

        setHtml += "<div style='height:40px;'></div>";
        setHtml += "<div class='iconSetFoot' style=''>";
        setHtml += "<ul class='iconSetFootLi'>";
        setHtml += "</ul>";
        setHtml += "</div>";
        setHtml += "</div>";

        if (document.querySelector("#setMask")) return;

        this.createElement("div", "zhmMenu");

        let zhmMenu = document.getElementById("zhmMenu");

        zhmMenu.innerHTML = setHtml;

        let timerZhmIcon = setInterval(function () {
            if (document.querySelector("#zhmMenu")) {
                clearInterval(timerZhmIcon); // 取消定时器

                // 获取之前的开关状态
                let previousState = GM_getValue("switchVoice", true);
                // 如果之前的状态存在，设置开关的状态
                if (previousState) {
                    document.getElementById("togBtn").checked = previousState == true;
                }
                // 当开关被点击时，切换状态并保存到本地存储
                document
                    .getElementById("togBtn")
                    .addEventListener("change", function () {
                        previousState = this.checked;
                        console.log("语音开关：", this.checked);
                    });

                // 获取之前的开关状态
                let gmState = GM_getValue("GM_notice", true);
                // 如果之前的状态存在，设置开关的状态
                if (gmState) {
                    document.getElementById("gmTogBtn").checked = gmState == true;
                }
                // 当开关被点击时，切换状态并保存到本地存储
                document
                    .getElementById("gmTogBtn")
                    .addEventListener("change", function () {
                        gmState = this.checked;
                        console.log("通知弹窗开关：", this.checked);
                    });

                const btn = document.querySelector("#btn");
                const radioButtons =
                    document.querySelectorAll('input[name="lang"]');

                let selectedLang = GM_getValue("LANG", "zh-HK");
                radioButtons.forEach(function (checkbox) {
                    checkbox.addEventListener("click", function (event) {
                        if (checkbox.checked) {
                            selectedLang = checkbox.value;
                            GM_setValue("LANG", selectedLang);
                            console.log("选中了选项:", checkbox.value);
                        } else {
                            console.log("取消选中选项:", checkbox.value);
                        }
                    });
                });

                const btn2 = document.querySelector("#btn2");
                const sb = document.querySelector("#framework");
                btn2.onclick = (event) => {
                    event.preventDefault();
                    // show the selected index
                    //alert(sb.selectedIndex);
                    GM_setValue("RATE", sb.value);
                };
                sb.onchange = (event) => {
                    event.preventDefault();
                    // show the selected index
                    //alert(sb.selectedIndex);
                    GM_setValue("RATE", sb.value);
                    console.log("语速选择:", sb.value);
                };
                document
                    .querySelector(".iconSetSave")
                    .addEventListener("click", () => {
                        //location.href = location.href;
                        var elem = document.getElementById("zhmMenu"); // 按 id 获取要删除的元素
                        elem.parentNode.removeChild(elem); // 让 “要删除的元素” 的 “父元素” 删除 “要删除的元素”
                        GM_setValue("switchVoice", true);
                        let bSwitch = previousState == true ? "语音播报已开启" : "语音播报已关闭";
                        let strGMSwitch = gmState == true ? "通知弹窗已开启" : "通知弹窗已关闭";
                        GM_setValue("GM_notice", gmState);
                        let numRate = GM_getValue("RATE", 1);
                        let strLang = GM_getValue("LANG", "zh-HK") === "zh-CN" ? "国语" : "粤语";
                        let sTxt = bSwitch + '，'+ strGMSwitch 
                            +"，播报语言设置为" + strLang + "，语速设置为" + numRate;
                        console.log(sTxt)
                        speak(
                            {
                                text: sTxt,
                            },
                            function () {
                                console.log("语音播放结束：" + sTxt);
                            },
                            function () {
                                console.log("语音开始播放");
                            }
                        );
                        GM_setValue("switchVoice", previousState);
                    });
            }
        });
    }

    createElement(dom, domId) {
        var rootElement = document.body;
        var newElement = document.createElement(dom);
        newElement.id = domId;
        var newElementHtmlContent = document.createTextNode("");
        rootElement.appendChild(newElement);
        newElement.appendChild(newElementHtmlContent);
    }
}

var baseClass = new BaseClass();

/**
 * @description 文字转语音方法
 * @public
 * @param { text, rate, lang, volume, pitch } object
 * @param  text 要合成的文字内容，字符串
 * @param  rate 读取文字的语速 0.1~10  正常1
 * @param  lang 读取文字时的语言
 * @param  volume  读取时声音的音量 0~1  正常1
 * @param  pitch  读取时声音的音高 0~2  正常1
 * @returns SpeechSynthesisUtterance
 */
function speak(
    { text, speechRate, lang, volume, pitch },
    endEvent,
    startEvent
) {
    if (!window.SpeechSynthesisUtterance) {
        console.warn("当前浏览器不支持文字转语音服务");
        return;
    }

    if (!text) {
        return;
    }
    let onoff = GM_getValue("switchVoice", true);
    if (onoff != true) {
        return;
    }

    let setlang = GM_getValue("LANG", "zh-CN");
    let setrate = GM_getValue("RATE", 1);
    const speechUtterance = new SpeechSynthesisUtterance();
    speechUtterance.text = text;
    speechUtterance.rate = speechRate || setrate;
    speechUtterance.lang = lang || setlang;
    speechUtterance.volume = volume || 1;
    speechUtterance.pitch = pitch || 1;
    speechUtterance.onend = function () {
        endEvent && endEvent();
    };
    speechUtterance.onstart = function () {
        startEvent && startEvent();
    };
    var timeFun = window.setInterval(function () {
        window.clearInterval(timeFun);
        speechSynthesis.speak(speechUtterance);
    }, 500);

    return speechUtterance;
}

function shim_GM_notification() {
    if (typeof GM_notification === "function") {
        return;
    }
    window.GM_notification = function (ntcOptions) {
        checkPermission();

        function checkPermission() {
            if (Notification.permission === "granted") {
                fireNotice();
            } else if (Notification.permission === "denied") {
                console.log(
                    "User has denied notifications for this page/site!"
                );
                return;
            } else {
                Notification.requestPermission(function (permission) {
                    console.log("New permission: ", permission);
                    checkPermission();
                });
            }
        }

        function fireNotice() {
            if (!ntcOptions.title) {
                console.log("Title is required for notification");
                return;
            }
            if (ntcOptions.text && !ntcOptions.body) {
                ntcOptions.body = ntcOptions.text;
            }
            var ntfctn = new Notification(ntcOptions.title, ntcOptions);

            if (ntcOptions.onclick) {
                ntfctn.onclick = ntcOptions.onclick;
            }
            if (ntcOptions.timeout) {
                setTimeout(function () {
                    ntfctn.close();
                }, ntcOptions.timeout);
            }
        }
    };
}

//function test(){location.reload()}
function reloadPage() {
    var refreshInterval = 5 * 1000; //* 3600 * 24; // 设置刷新间隔时间（单位：秒）
    //test();
    //location.reload();
    var timeFun = window.setInterval(function () {
        location.reload();
        window.clearInterval(timeFun);
    }, refreshInterval);
}

var init_flag = 0;
function append_notify(res) {
    var status = false;
    var changed = 0;
    for (let each in res.data.list) {
        // for room status
        var isLive = !res.data.list[each].videoLoop;
        status = isLive && res.data.list[each].show_status == 1;
        if (!isLive) {
            console.log(
                res.data.list[each].nickname +
                "-" +
                res.data.list[each].room_id +
                "-" +
                isLive +
                "-" +
                status +
                "-视频轮播ing"
            );
        }
        if (!(res.data.list[each].room_id in save)) {
            save[res.data.list[each].room_id] = status;
            if (init_flag == 1) {
                changed = 1;
            }
        } else if (save[res.data.list[each].room_id] != status) {
            save[res.data.list[each].room_id] = status;
            let strStatus = status == true ? "开播了" : "下播了";
            var notificationDetails = (function () {
                var tempUrl = res.data.list[each].url;
                speak(
                    {
                        text: res.data.list[each].nickname + strStatus,
                    },
                    function () {
                        console.log("语音播放结束");
                    },
                    function () {
                        console.log("语音开始播放");
                    }
                );
                return {
                    text: "点击通知快速传送",
                    title: res.data.list[each].nickname + strStatus,
                    image: res.data.list[each].avatar_small,
                    //timeout:    60000,
                    onclick: function () {
                        console.log("Notice clicked.");
                        GM_openInTab(baseURL + tempUrl, false);
                        //window.focus ();
                    },
                };
            })();
            wrap_GM_notification(notificationDetails);
            //下播 开播都刷新 反正状态变了都刷新
            changed = 1;
        }

        // for room name changing
        if (!(res.data.list[each].room_id in save_name)) {
            save_name[res.data.list[each].room_id] =
                res.data.list[each].room_name;
            changed = 1;
        } else if (
            save_name[res.data.list[each].room_id] !=
            res.data.list[each].room_name
        ) {
            save_name[res.data.list[each].room_id] =
                res.data.list[each].room_name;
            var notificationDetails_name = (() => {
                var tempUrl = res.data.list[each].url;
                speak(
                    {
                        text:
                            res.data.list[each].nickname +
                            " 更改了房间标题" +
                            res.data.list[each].room_name,
                    },
                    function () {
                        console.log("语音播放结束");
                    },
                    function () {
                        console.log("语音开始播放");
                    }
                );
                return {
                    text: res.data.list[each].room_name,
                    title: res.data.list[each].nickname + " 更改了房间标题",
                    image: res.data.list[each].avatar_small,
                    //timeout:    60000,
                    onclick: function () {
                        console.log("Notice clicked.");
                        GM_openInTab(baseURL + tempUrl, false);
                        //window.focus ();
                    },
                };
            })();
            wrap_GM_notification(notificationDetails_name);
            changed = 1;
        }
    }
    if (init_flag != 0 && changed == 1) {
        reloadPage();
    }
    init_flag = 1;
    console.log("Following rooms checked");
}

function check() {
    console.log("Following rooms checking");
    GM_xmlhttpRequest({
        method: "GET",
        url: `https://www.douyu.com/wgapi/livenc/liveweb/follow/list?sort=0&cid1=0`,
        onload: (response) => {
            var res = JSON.parse(response.responseText);
            append_notify(res);
        },
    });
}

function wrap_GM_notification(param) {
    let bGMnotice = GM_getValue("GM_notice", true);
    if (bGMnotice) {
        GM_notification(param);
    } else {
        console.log("GM_notification disabled");
    }
}

check();
function notifyTitle(s) {
    wrap_GM_notification({
        text: "斗鱼开播提醒",
        title: s,
        timeout: 1800,
        image: "https://img.douyucdn.cn/data/yuba/admin/2018/08/13/201808131555573522222945055.jpg?i=31805464339f469e0d3f992e565e261803",
        onclick: function () {
            console.log("Notice clicked.");
            GM_openInTab("https://www.douyu.com", false);
            //window.focus ();
        },
    });
}
notifyTitle("斗鱼开播提醒启动了");
//window.onbeforeunload = function(event){notifyTitle('开播提醒已退出')}
//window.onunload = function(event) {notifyTitle('斗鱼开播提醒已退出')}
window.setInterval(check, 10000);
