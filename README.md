# Aura RPG - Unreal Engine 5.5 기반 멀티플레이 RPG

이 프로젝트는 언리얼 엔진 5.5에서 `Gameplay Ability System (GAS)`과 


`Dedicated Server`, `AWS 연동`, `UI 및 AI 시스템`, `레벨 스트리밍`을 통합하여 만든 멀티플레이 RPG 게임 포트폴리오입니다.

Stephen Ulibarri님의 Udemy 강의를 기반으로 학습 및 개발을 진행하였으며, 그 위에 자체 시스템을 확장하여 제작했습니다.

</br>

플레이 영상: [https://youtu.be/4x9iSUawjKQ](https://youtu.be/4x9iSUawjKQ)

</br>

---

## 설계 방향

- **Low Coupling**
  - 모듈과 클래스 간 가능한 단방향 참조
  - 인터페이스 사용 (예: AttributeSet에서 CharacterAura클래스에 접근할 때 `AttributeSet → IPlayerInterface` 등)

- **MVC UI 구조**
  - 데이터 → 컨트롤러 → UI 바인딩
  - 델리게이트 및 브로드캐스트를 통한 변화 전달

- **확장성 있는 GAS 설계**
  - 모든 GA는 상속 기반으로 분리 구성
  - `GameplayTag`와 `DataAsset`로 유연한 구성 가능

</br>

---

## 프로젝트 구조 및 모듈

### 1. Aura 게임 모듈
- GAS 기반 Ability 시스템
- UI 시스템 (MVC 패턴)
- Enhanced Input 사용
- AI 및 내비게이션
- 레벨 스트리밍 및 스킬 시스템 등 전반적인 게임 기능 포함

### 2. DedicatedServer 모듈
- AWS Cognito 및 DynamoDB 연동
- Player 데이터의 저장/불러오기 기능
- AWS Gamelift와의 세션 통신 처리

</br>

---

## GAS 시스템 구성

### GameplayTag 및 DataAsset 기반으로 구성
- 캐릭터 클래스, 스킬, Attribute 정보는 모두 `GameplayTag`와 `DataAsset`을 통해 정의되어 있어 확장이 용이합니다.

### Data 구성 예

#### DA_AbilityInfo

<img src="README_Images/Data-GameMode/DA_AbilityInfo.jpg" width="85%" />

#### DA_CharacterClassInfo

<img src="README_Images/Data-GameMode/DA_CharacterClassInfo.jpg" width="85%" />

</br>
</br>

### Attribute 시스템
- `PrimaryAttribute`: Strength, Intelligence, Resilience, Vigor 등 캐릭터 기본 능력
- `SecondaryAttribute`: Armor, BlockChange, CriticalHit, MaxHealth, MaxMana 등은 `PrimaryAttribute`를 기반으로 계산됩니다.
- `MaxHealth`와 `MaxMana`는 `ModMagnitudeCalculation (MMC)` 클래스를 통해 동적으로 계산됩니다.
- 플레이어의 패시브 스킬(`GA_ListenForEvent`)은 이벤트 기반으로 특정 태그를 감지하고 `SetByCaller` 방식으로 속성값을 적용합니다.

</br>

#### Attribute
<img src="README_Images/Attribute/AttributeInfo.jpg" width="80%" />

</br>


---

## 스킬 시스템 구성

### 구현된 스킬

#### 액티브
- FireBolt: FireBolt 스킬, 레벨업시 발사 숫자가 증가 및 타겟 추적 기능 포함, 화염 디퍼브(일정시간 동안 일정한 지속 데미지)
- Electrocute: Target으로의 지속 데미지 Beam을 생성, 스턴 디버프 포함
- ArcaneShard: 선택한 지역에 마법 조각을 생성, 범위 피해

#### 패시브
- Siphon: 피해량에 따라 체력/마나 흡수

#### 적 몬스터 스킬
- 근접 공격, 원거리 공격, FireBolt, 소환 스킬 등 다양한 형태 구현

### 스킬 발동의 흐름 (예: FireBolt)
1. 키 입력 → 태그 전달 → `ASC`가 해당 GA 활성화
2. `GA` 내부에서 타겟 탐색, 몽타쥬 재생, 태그 대기, 몽타쥬 재생 중 이벤트 태그 전달, 태그를 받아서 Projectile 생성, 발사
3. Projectile이 충돌 시 DamageEffect를 생성하고 적용함
4. 데미지 계산은 ExecCalc 클래스에서 수행

### 스킬 포인트와 스킬 장착
- 스킬 포인트를 소비해서 스킬을 배우거나 스킬 레벨을 업그레이드
- 원하는 키에 매핑해서 장착후 사용가능

#### 스킬 장착 예
<img src="README_Images/추가/img_SkillEquiped.jpg" width="50%" />

</br>
</br>

---

## 데미지 및 전투 시스템

### Damage 흐름 (예: FireBolt)
- Projectile 생성 시 `FDamageEffectParams`를 통해 소스/타겟 정보, 데미지, 버프/디버프 정보등을 전달함
- 타겟팅 계산 - UAuraTagertedGA 클래스, 데미지 계산 - UAuraDamageGiveGA 클래스 등
- 충돌 시 `GESpec`을 생성하고 TargetASC에 적용
- ExecCalc 클래스에서:
  - 기본 데미지 계산
  - 방어력 및 속성 저항 적용
  - 크리티컬, 상태 이상 확률 적용
  - RadialDamage 및 디버프 확률 적용

- 계산된 결과는 `AttributeSet::PostExecute`에서 적용되며 이때 HitResult, Knockback, Die 처리가 이뤄집니다.

</br>

#### DamegeEffectParameter를 통해 전달하는 정보 예
<img src="README_Images/FireBolt 전달방식/VS_DamageEffectParams.jpg" width="70%" />

</br>


---

## 레벨업 및 경험치 시스템

- AuraPlayerState 클래스에서 레벨, XP, AttributePoint, SpellPoint등을 관리
- 몬스터 처치 → (AttributeSet 클래스에서) `XpReward` 태그를 `GA_ListenForEvent` 에 전달
- 경험치 누적 → MaxXP 도달 시 레벨업
- 레벨업시
  - `DA_LevelUpInfo`에 정의된 데이터 기반으로 `AttributePoint`, `SpellPoint` 지급
  - MMC 클래스가 레벨 기반으로 MaxHealth, MaxMana 재계산

</br>

#### ListenForEvent 예
<img src="README_Images/추가/ListenForEvent.jpg" width="90%" />

</br>

---

## 인벤토리 및 아이템 시스템

- 아이템 유형: 소비형(획득 + 상정구매), 컬렉션형
- 아이템 획득 경로: 몬스터 처치, 보스 처치 후 상자 생성, 상점 구입
- 소비형 아이템은 단축키로 사용 가능하며 GE로 자신에게 효과 적용
- 아이템 데이터도 `AuraPlayerState 클래스`에서 관리

#### Store Widget
<img src="README_Images/추가/Store.jpg" width="50%" />

</br>


---

## UI 시스템

- 모든 UI는 MVC 패턴으로 관리됩니다.
- 예: AttributeMenu
  - Controller는 데이터의 변화를 Listen하고 UI 위젯에 전달
  - `AuraAttributeSet → AttributeMenuWidgetController → AttributeMenuWidget`
  - UI에서 버튼 클릭 → Controller 함수 호출 → Data 처리 → 브로드캐스트 → 다시 UI에 반영

</br>

---

## Enemy AI 시스템

- 언리얼 Behavior Tree 시스템 기반
- 주요 판단 요소:
  - 타겟 탐색 여부
  - HitReact, Stun 등 상태 이상 여부
  - 거리 기반 공격 가능 여부
- 행동: 이동, 대기, 근거리/원거리 공격, 소환 등

#### BT Tree 예
<img src="README_Images/AI/img_BehaviorTree1.jpg" width="80%" />

</br>

---

## 맵 전환 및 스트리밍 시스템

- `MapEntrance` 오버랩 시 서버에서 다음 맵을 비동기 로드
- `PlayerStartTag`에 따라 플레이어 위치 이동, Client RPC를 호출
- 클라이언트에서 해당 맵을 로드
- 로드 완료되면 이전맵을 언로드

</br>

---

## CheckPoint 및 세이브 시스템

- `SavePoint`: 오버랩 시 위치 저장
- `MapEntrance`: 맵 이동 + 위치 저장
- 서버가 AWS를 통해 저장 요청
- 플레이어 사망 시 PlayerSession을 만들고 데이터를 로드하는 흐름을 다시 따라감 (마지막 저장 위치로 리스폰)

</br>
</br>
</br>

---

## AWS 연동 구조

### 사용 서비스
- Cognito (회원가입, 로그인, 세션 관리)
- DynamoDB (데이터 저장 및 불러오기)
- AWS Lambda 생성
- EC2 플릿 생성 후 테스트

### DynamoDB 테이블 구성
- `UserInfo`, `PlayerData`, `Attributes`, `Inventory`, `Skills`
- PrimaryKey는 `Cognito`의 `sub` 값을 사용

</br>

#### Table 예
<img src="README_Images/AWS/Tables.jpg" width="60%" />

</br>

#### PlayerInfo Data 예
<img src="README_Images/AWS/TB_Attributes.jpg" width="60%" />

</br>

#### Skill Data 예
<img src="README_Images/AWS/TB_Skill.jpg" width="60%" />

</br>

</br>

### 주요 매니저 클래스
- `HTTPRequestManager`: HTTP 요청/응답 처리
- `PortalManager`: 회원가입, 로그인, 이메일 인증
- `GameSessionManager`: Gamelift 세션 생성 및 관리
- `SaveManager`: 플레이어 데이터 저장 및 로딩

### 흐름
1. SignUp + Confirm → `sub` 생성 및 DynamoDB에 초기값 저장
2. SignIn → 위치 및 레벨 정보 불러오기
3. Join → GameSession 및 PlayerSession 생성 → 서버 접속
4. ServerMap의 GameMode에서 LoadData
5. 플레이 중 CheckPoint 시점에 SaveManager를 통해 AWS에 저장

</br>
</br>



## 요약 정보

| 항목         | 내용                             |
|--------------|----------------------------------|
| 엔진         | Unreal Engine 5.5                |
| 핵심 시스템  | GAS, AI, Enhanced Input, AWS     |
| 네트워크     | Dedicated Server                 |
| 저장 방식    | AWS Cognito + DynamoDB           |
| 설계 방식    | MVC 패턴, 인터페이스 기반 참조  |
| 참고 자료    | Stephen Ulibarri GAS 강의         |
| 플레이 영상  | https://youtu.be/4x9iSUawjKQ     |
