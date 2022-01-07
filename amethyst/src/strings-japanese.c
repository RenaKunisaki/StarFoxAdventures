#include "main.h"

const char *strings_Japanese[] = {
    "住所", //memory address
    "住所", //memory address
    "アタッチポイント", //show attachment points
    "オーディオ設定", //change audio settings
    "オーディオ効果音", //sound effects
    "オーディオストリーミング", //change volume of streaming audio
    "オート", //automatic
    "オートセーブ", //save automatically
    "バックパック", //whether player wears a backpack
    "戻る", //return to previous
    "大きい", //PDA map size: big
    "鳥瞰図", //have camera look down from sky
    "Bit", //GameBit (not the same as binary bits)
    "ブラーフィルター", //enable a blur over the screen
    "境界", //boundaries
    "プレイヤーに向けて移動", //move selected object to player
    "カメラ", //camera settings
    "カメラコントロール", //camera control mode
    "カメラ座標", //camera coordinates
    "カメラ X", //camera control mode for X axis
    "カメラ Y", //camera control mode for Y axis
    "カテゴリー", //category ID
    "チート", //cheat functions
    "CHILD", //child object
    "リスタートポイント解除", //clear the "restart point"
    "カラーフィルター", //change color filtering
    "コントロー尺度", //change color scaling
    "コントローラー", //which gamepad
    "コントロール設定", //change controls
    "ゲームをクラッシュする", //cause a fatal error (for testing)
    "現在ご飯", //current food/energy
    "現在HP", //current health
    "現在ライフ", //current extra lives
    "現在MP", //current magic power
    "カットシーン音量", //audio volume during scenes
    "Day", //day of month
    "デバッグ", //debug menu
    "デバックオブジェクト", //show objects that are normally hidden
    "デバックテキスト", //show debugging text
    "定義番号", //definition number
    "削除", //delete
    "距離", //distance
    "DLL", //DLL (Dynamically Linked Library)
    "編集", //edit
    "ファイル編集", //edit file
    "メモリエディット", //edit memory
    "オブジェクト定義を編集", //edit object definition
    "プレイヤー状態の編集", //edit player's state
    "トリッキー状態の編集", //edit Tricky's state
    "状態編集", //edit object's state
    "エフェクト", //effects
    "環境", //environment (time, weather...)
    "イベント", //EVENT
    "終了する", //exit this menu
    "速い", //move faster
    "ファイル", //FILE (on a computer)
    "フィルター B", //filter setting for blue
    "フィルター G", //filter setting for green
    "フィルター R", //filter setting for red
    "一人称ビュー", //have camera look from player's head
    "FLAG", //FLAG (option)
    "フォーカスポイント", //show camera focus points
    "フォローポインタ", //follow pointer
    "Font Test", //display all fonts
    "FOV", //field of view
    "フォックス", //Fox player character
    "FPSメーター", //Frames Per Second
    "自由に動く", //let camera move anywhere
    "フリームーブ", //move the player anywhere
    "敵フリーズ", //enemies can't move (effect of Ice Blast spell)
    "Fuel Cell Compass", //PDA mode: show nearby fuel cells
    "毛のエフェクト", //Fur effect
    "GameBit変更", //indicate when a GameBit is changed
    "GameBits", //game flags
    "ゲーム設定", //change game settings
    "ゲーム速度", //change overall game speed
    "ゲームステート", //change overall game state
    "最後にセーブした場所に移動", //move to the last saved location
    "移動", //move to this object
    "リスタートポイントに移動", //move to the "restart point"
    "ヘッドホン", //sound mode: headphones
    "ヒープの状態", //show details about memory allocation
    "熱表現", //enable a heat wave effect on screen
    "ヒットボックス", //show hit areas
    "接触領域", //hitboxes
    "HUD", //heads-up display
    "ID", //ID number
    "HP無限", //infinite health
    "ライフ無限", //infinite extra lives
    "お金無限", //infinite money (scarabs)
    "MP無限", //infinite magic
    "トリッキーエナジー無限", //infinite Tricky energy
    "情報", //PDA mode: show info about nearby objects
    "触れたオブジェクト", //object the player is interacting with
    "反転", //opposite direction (for camera)
    "プレイヤーkill", //kill the player (for testing)
    "クリスタル", //Krystal player character
    "言語", //change text language
    "ライト", //debug: examine lights
    "名鑑", //open a list of objects
    "Load EnvFX", //load environment effects
    "Load  Flags", //flags to use when loading (extra space for alignment)
    "Load Map", //load a map's assets
    "HP低下 フラッシュ", //flash the HP meter when HP is low
    "HP低下 音", //play a sound when HP is low=
    "マップの行為番号", //"act numbers" of each map
    "マップグリッド", //show the map grid on-screen
    "マップのID", //ID number of map
    "マップ", //map
    "マップの不透明度", //opacity of PDA map
    "マップの寸法", //size of PDA map
    "マップの状態", //state of each map
    "マップのズーム", //zoom level of PDA map
    "マスター", //master debug text enable
    "最大ご飯", //maximum food/energy
    "最大HP", //maximum health
    "最大ライフ", //maximum extra lives
    "最大MP", //maximum magic power
    "メモリの状態", //show stats for memory allocation
    "その他", //miscellaneous
    "モデル", //3D model
    "モード", //mode
    "お金", //current money (scarabs)
    "モノクロ", //show everything in monochrome
    "単核症", //sound mode: mono
    "モーションブラー", //enable a motion blur effect
    "動かす", //move the cursor
    "言付け", //message queue
    "曲音量", //volume for background music
    "名", //name
    "N/A", //not applicable
    "なし", //none
    "NO OBJDEF", //no object definition data
    "正常", //normal setting
    "OBJDEF", //object definition
    "オブジェクト", //game object
    "オブジェクト", //show game objects
    "オブジェクトのID", //object ID
    "オブジェクト番号", //object number
    "オブジェクト番号", //object number
    "Obj Seq", //show object sequences
    "Off", //off
    "On", //on
    "元の位置", //original position
    "メモリ不足", //out of memory
    "ページ", //move to another page
    "パラメータ", //parameter
    "パラメータ", //parameters
    "PARENT", //parent object
    "パーティクルFX", //particle effects
    "PDAメニュー", //PDA menu (main menu title)
    "PDA設定", //PDA settings
    "パフォーマンスメーター", //performance meters
    "パフォーマンス", //show performance information
    "持つ", //pick up this object
    "プレイヤー座標", //player coordinates
    "プレイヤー", //object menu instructions
    "プレイヤー設定", //settings for player character
    "プレイヤーの状態", //player character's state
    "プレイヤー", //which player to use
    "Play Count", //number of times played
    "SE再生", //play sound effect
    "曲再生", //play a song
    "ストリーム音源再生", //play audio stream
    "位置", //position
    "マップリロード", //reload the map
    "レンダリング", //debug settings for rendering
    "復活ポイント", //show the "restart point" coordinates
    "乗る", //start riding this object
    "RNG", //random number generator
    "振動ぼかし", //blur the screen when rumble is active
    "振動", //controller vibrate function
    "シーケンス%d実行", //run sequence
    "データの保存", //saving the game
    "SEQ", //sequence number
    "プレイヤーとして設定", //make this object the player object
    "カメラフォーカス設定", //make the camera look at this object
    "リスタートポイント設定", //save currewnt position as "restart point"
    "セーブポジション設定", //save current position as "save point"
    "SFX音量", //sound effect volume
    "示す", //show something
    "Skip", //skip this scene
    "スロット", //slot number
    "小さな", //PDA map size: small
    "並べ替え", //sorting the list
    "区分する", //Sort a list
    "サウンド音量", //choose a sound mode
    "Spawn Flags", //flags to use when spawning
    "オブジェクト召喚", //spawn an object
    "召喚", //perform the spawn
    "魂エフェクト", //show everything in effect used for Krazoa Spirit vision
    "状態", //object state data
    "止まる", //have camera stay in place
    "ステレオ", //sound mode: stereo
    "曲を全停止", //stop all sounds
    "サブタイトル", //enable subtitles
    "サラウンドサウンド", //sound mode: surround
    "大小", //2-letter abbreviation for Size
    "源", //1-letter abbreviation for Table
    "目録", //database table
    "時間", //time of day
    "トリッキー", //the character named Tricky
    "一意のID", //unique ID
    "わからない07", //unknown field 7
    "不明なポイント", //show ??? points
    "すべてアンロック", //give player all items, etc
    "値", //value of a variable
    "ビデオ設定", //settings for display
    "フォント表示", //show all fonts
    "文章表示", //show all GameTexts
    "テクスチャ表示", //show all textures
    "ワープ", //warp to another map
    "ワイドスクリーン", //enable widescreen display
    "位置", //coordinate
    NULL
};
