if test -d "/.snapshot/"`date +"%Y-%m-%d"`; then
    echo "快照已存在."
else
    echo "正在创建快照..."
    sudo btrfs subvolume snapshot / /.snapshot/`date +"%Y-%m-%d"`
    echo "创建完成."
fi
# echo $directory