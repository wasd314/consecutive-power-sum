#!/bin/bash

# ../rime-out/tests/*.in を ./in/*.txt に，
# ../rime-out/tests/*.diff を ./diff/*.txt にコピーする

# スクリプトが存在するディレクトリ
SCRIPT_DIR="$(realpath "$(dirname "$0")")"

# コピー元
SOURCE_DIR="${SCRIPT_DIR}/../rime-out/tests"

# 存在チェック
if [[ ! -d "${SOURCE_DIR}" ]]; then
    echo "エラー: ソースディレクトリ ${SOURCE_DIR} が存在しません" >&2
    exit 1
fi

# 処理する拡張子のリスト
EXTENSIONS=("in" "diff")

for ext in "${EXTENSIONS[@]}"; do
    target_dir="${SCRIPT_DIR}/${ext}"

    mkdir -p "${target_dir}"
    rm -f "${target_dir}"/*

    # 該当する拡張子のファイルを.txtに変換してコピー
    for file in "${SOURCE_DIR}"/*.${ext}; do
        if [[ -f "$file" ]]; then
            basename_without_ext=$(basename "$file" .${ext})
            target_file="${target_dir}/${basename_without_ext}.txt"
            cp "$file" "$target_file"
        fi
    done
done
