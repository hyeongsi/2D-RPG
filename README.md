# 2D RPG

<br>

## 프로젝트 소개
<div>
<img width="400" src="https://user-images.githubusercontent.com/71202869/201508722-814bf367-2718-4097-9c04-a009fc23c4df.png"/>
<img width="400" src="https://user-images.githubusercontent.com/71202869/201508750-4eac7268-a397-482a-b695-e186d9772cc8.png"/>
<img width="400" src="https://user-images.githubusercontent.com/71202869/201508822-ae36891b-59d0-4b0b-a8f8-8e392d1b9fbd.png"/>
<img width="400" src="https://user-images.githubusercontent.com/71202869/201508886-e2b5bbef-f566-406b-8bf3-9442a307f819.png"/>
</div>

## 개요
+ 맵 에디터의 필요성을 경험해보기 위해 제작

+ AStar 알고리즘에 대해 학습하고 적용하기
<br>

## 기술 스택
* Cpp

* Win api

* Visual Studio

<br>

## 시연 영상
<details>
  <summary>게임 시작</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511389-b1c7a0f4-9406-46a7-9bf8-58ff9d67c711.gif"/>
</details>
<details>
  <summary>전투</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511454-85c5eacc-7376-4fc8-96fe-f9976ffb29b0.gif"/>
</details>
<details>
  <summary>상호작용 및 씬이동</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511444-6a5215c9-a0cb-455d-96ce-da17d75fbeb3.gif"/>
</details>
<details>
  <summary>상점 및 인벤토리</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511423-6ec2bbd7-6509-4a36-a1a7-30c7cce7a32f.gif"/>
</details>
<details>
  <summary>맵에디터 사용</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511413-0af93a54-6c2b-46fd-bfb2-948757383d6d.gif"/>
</details>
<details>
  <summary>맵에디터 불러오기</summary>
  <img width="976" src="https://user-images.githubusercontent.com/71202869/201511395-7c268e0b-1d34-4a03-ab72-8685fa94cce6.gif"/>
</details>

<br>

## 아쉬운점
* 계속해서 씬을 이동할때 여러 상호작용들이 일어나는데 아직은 오브젝트가 별로 없어 체감이 안되지만<br/>
오브젝트가 많았다면 프레임 드랍 문제가 발생했을 것이다. 더 나은 구조로 만들 수 있지 않았을까 하는 후회가 남는다.

* 맵 -> 필드 아이템 -> 캐릭터 -> ui 순으로 렌더링했고, 오브젝트마다 렌더링 순서를 정하여 바꿀 수 있었다면<br/>
더 자연스러웠겠지만 프로젝트 일정, 제작한 렌더링 구조로 구현하기엔 처음부터 다 고쳐야하기 때문에 <br/>
다음 프로젝트에서 해당 내용을 생각하며 구현하기로 함

* UI 관리 클래스를 만들어 관리 하지 못해 ui 생성 및 관리 확장성 부분에 다소 아쉬움이 남는다.

<br/>

## 기타 / 조작법
<table>
  <tr>
    <td><b>방향키</b></td>
    <td><b>space</b></td>
    <td><b>ctrl</b></td>
    <td><b>i</b></td>
    <td><b>esc</b></td>
  </tr>
  <tr>
    <td>이동</td>
    <td>상호작용</td>
    <td>공격</td>
    <td>인벤토리</td>
    <td>게임 옵션</td>
  </tr>
</table>
