<RCC>
    <qresource prefix="/">
${t.(`find . -name "*.png" | xargs -i -d '\n' echo -e '        <file>'{}'</file>'`)}
    </qresource>
</RCC>
