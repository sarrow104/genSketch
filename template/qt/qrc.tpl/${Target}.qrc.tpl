<RCC>
    <qresource prefix="/">
${t.(`find $path -name "*.png" | xargs -i -d '\n' echo -e '        <file>'{}'</file>'`)}
    </qresource>
</RCC>
