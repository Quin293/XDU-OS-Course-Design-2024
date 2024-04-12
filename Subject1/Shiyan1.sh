#!/bin/bash

#检查格式
if [ $# -ne 2 ]; then
	echo "FORMAT ERROR"
	exit 1
fi

#定义输入参数
file=$1
flag=$2

#生成shell脚本
echo "#!/bin/bash" > NewShell.sh
echo "if [ \"$flag\" = \"read\" ]; then" >> NewShell.sh
echo "	cat \"$file\"" >> NewShell.sh
echo "fi" >> NewShell.sh
echo "if [ \"$flag\" = \"write\" ]; then" >> NewShell.sh

#输入要读入的参数
echo "	read -p \"Enter content to write: \" content" >> NewShell.sh
echo "	echo \"\$content\" >> \"$file\"" >> NewShell.sh
echo "fi" >> NewShell.sh

#将新脚本设为可执行
chmod +x NewShell.sh

#使用strace观察执行结果
strace -o output.txt ./NewShell.sh "$file" "$flag"

#输出结果
echo "Strace Output:"
cat output.txt

#删除新文件以便再次执行
rm NewShell.sh output.txt
