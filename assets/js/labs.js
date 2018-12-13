function run(post, target) {
    var converter = new showdown.Converter();

    var oRequest = new XMLHttpRequest(),
        sURL = post,
        target = document.getElementById(target),
        allText = "";

    oRequest.open("GET",sURL,false);
    oRequest.setRequestHeader("Accept", "text/html");
    oRequest.onreadystatechange = function () {
        if (oRequest.readyState == 4) {
            if (oRequest.status == 200) {
                allText = oRequest.responseText;
                html = converter.makeHtml(allText);
                target.innerHTML = html;
            }
        }
    }
    oRequest.send(null);
}

run("assets/posts/about.md", "about")
run("assets/posts/implementation.md", "implementation")
run("assets/posts/user-study.md", "user-study")
run("assets/posts/resources.md", "resources")
