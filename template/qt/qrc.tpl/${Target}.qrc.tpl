<RCC>
    <qresource prefix="/">
${t.(`find . -name "${pattern}" | xargs -i -d '\n' echo -e '        <file>'{}'</file>'`)}
    </qresource>
</RCC>
