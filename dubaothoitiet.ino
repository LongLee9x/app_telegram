<?php 
header(<Content-Type: application/json; charset=utf-8<);

$url_qtri = <http://api.openweathermap.org/data/2.5/weather?id=1582926&appid=**APIKEY_CỦA_OPENWEATHERMAP_API**&units=metric&lang=vi<;
$content_tri = file_get_contents($url_qtri);
$json_qtri = json_decode($content_tri, true);

$token = "5005939601:AAEooIdidd-1NkdIqYBPqPOMDyGtbcMpbT0";

$icon_qtri = $json_qtri[<weather<][0][<icon<];
switch ($icon_qtri) {
  case <01d<:
  case <01n<:
    $icon_qtri_url = <https://i.imgur.com/9CGjkOE.jpg<;
    break;
  case <02d<:
  case <02n<:
    $icon_qtri_url = <https://i.imgur.com/CRh04K2.jpg<;
    break;
  case <03d<:
  case <03n<:
    $icon_qtri_url = <https://i.imgur.com/zHuhzVo.jpg<;
    break;
  case <04d<:
  case <04n<:
    $icon_qtri_url = <https://i.imgur.com/riGvdrP.jpg<;
    break;
  case <09d<:
  case <09n<:
    $icon_qtri_url = <https://i.imgur.com/XHUnTV6.jpg<;
    break;
  case <10d<:
  case <10n<:
    $icon_qtri_url = <https://i.imgur.com/7ECPQGA.jpg<;
    break;
  case <11d<:
    $icon_qtri_url = <https://i.imgur.com/dMj6Rt9.jpg<;
    break;
  case <50d<:
  case <50n<:
    $icon_qtri_url = <https://i.imgur.com/j4jSE6N.jpg<;
    break;
  default:
    $icon_qtri_url = <http://openweathermap.org/img/wn/< . $json_qtri[<weather<][0][<icon<] . <@2x.png<;
    break;
}

$data_qtri = [
  <chat_id< => <1959776138<,
  <photo<   => $icon_qtri_url,
  <caption< => <Thời tiết ngày < . date(<d/m/Y<) . < tại Đông Hà, Quảng Trị có < . $json_qtri[<weather<][0][<description<] . <. Nhiệt độ là < . round($json_qtri[<main<][<temp<]) . <℃<
];
file_get_contents("https://api.telegram.org/bot$token/sendPhoto?" . http_build_query($data_qtri));

$url_hue = <http://api.openweathermap.org/data/2.5/weather?id=1580240&appid=**APIKEY_CỦA_OPENWEATHERMAP_API**&units=metric&lang=vi<;
$content_hue = file_get_contents($url_hue);
$json_hue = json_decode($content_hue, true);

$icon_hue = $json_hue[<weather<][0][<icon<];
switch ($icon_hue) {
  case <01d<:
  case <01n<:
    $icon_hue_url = <https://i.imgur.com/9CGjkOE.jpg<;
    break;
  case <02d<:
  case <02n<:
    $icon_hue_url = <https://i.imgur.com/CRh04K2.jpg<;
    break;
  case <03d<:
  case <03n<:
    $icon_hue_url = <https://i.imgur.com/zHuhzVo.jpg<;
    break;
  case <04d<:
  case <04n<:
    $icon_hue_url = <https://i.imgur.com/riGvdrP.jpg<;
    break;
  case <09d<:
  case <09n<:
    $icon_hue_url = <https://i.imgur.com/XHUnTV6.jpg<;
    break;
  case <10d<:
  case <10n<:
    $icon_hue_url = <https://i.imgur.com/7ECPQGA.jpg<;
    break;
  case <11d<:
    $icon_hue_url = <https://i.imgur.com/dMj6Rt9.jpg<;
    break;
  case <50d<:
  case <50n<:
    $icon_hue_url = <https://i.imgur.com/j4jSE6N.jpg<;
    break;
  default:
    $icon_hue_url = <http://openweathermap.org/img/wn/< . $json_hue[<weather<][0][<icon<] . <@2x.png<;
    break;
}

$data_hue = [
  <chat_id< => <1959776138<,
  <photo<   => $icon_hue_url,
  <caption< => <Thời tiết ngày < . date(<d/m/Y<) . < tại TP.Huế, Thừa Thiên Huế có < . $json_hue[<weather<][0][<description<] . <. Nhiệt độ là < . round($json_hue[<main<][<temp<]) . <℃<
];
file_get_contents("https://api.telegram.org/bot$token/sendPhoto?" . http_build_query($data_hue));
