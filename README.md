# Aura RPG - Unreal Engine 5.5 기반 멀티플레이 RPG 프로젝트

이 프로젝트는 언리얼 엔진 5.5에서 `Gameplay Ability System (GAS)`과 `Dedicated Server`, `AWS 연동`, `UI 및 AI 시스템`, `레벨 스트리밍`을 통합하여 만든 멀티플레이 RPG 게임입니다.

Stephen Ulibarri님의 Udemy 강의를 기반으로 학습 및 개발을 진행하였으며, 그 위에 자체 시스템을 확장하여 제작했습니다.

플레이 영상: [https://youtu.be/4x9iSUawjKQ](https://youtu.be/4x9iSUawjKQ)

---

## 프로젝트 구조 및 모듈

### 1. Aura 게임 모듈
- GAS 기반 능력 시스템
- UI 시스템 (MVC 패턴 적용)
- Enhanced Input 사용
- AI 및 내비게이션
- 레벨 스트리밍 및 스킬 시스템 등 전반적인 게임 기능 포함

### 2. DedicatedServer 모듈
- AWS Cognito 및 DynamoDB 연동
- Player 데이터의 저장/불러오기 기능
- AWS Gamelift와의 세션 통신 처리

---

## GAS 시스템 구성

### GameplayTag 및 DataAsset 기반 구성
- 캐릭터 클래스, 스킬, 어트리뷰트 정보는 모두 `GameplayTag`와 `DataAsset`을 통해 정의되어 있어 커스터마이징이 용이합니다.

### Attribute 시스템
- `PrimaryAttribute`: Strength, Intelligence, Vigor 등 캐릭터 기본 능력
- `SecondaryAttribute`: MaxHealth, MaxMana 등은 `PrimaryAttribute`를 기반으로 계산됩니다.
- `MaxHealth`와 `MaxMana`는 `ModMagnitudeCalculation (MMC)` 클래스를 통해 동적으로 계산됩니다.
- 플레이어 패시브 스킬(`GA_ListenForEvent`)은 이벤트 기반으로 특정 태그를 감지하고 `SetByCaller` 방식으로 속성값을 적용합니다.

---

## 스킬 시스템 구성

### 구현된 스킬

#### 액티브
- FireBolt: 기본 화염 투사체 스킬, 다수 발사 및 타겟 추적 기능 포함
- Electrocute: 연쇄 번개, 스턴 디버프 포함
- ArcaneShard: 선택한 지역에 마법 조각 생성, 범위 피해

#### 패시브
- Siphon: 피해량에 따라 체력/마나 흡수

#### 적 몬스터 스킬
- 근접 공격, 원거리 공격, FireBolt, 소환 스킬 등 다양한 형태 구현

### 스킬 발동 흐름
1. 키 입력 → 태그 전달 → `ASC`가 해당 GA 활성화
2. `GA` 내부에서 타겟 탐색, 몽타쥬 재생, 태그 대기, 투사체 발사
3. Projectile이 충돌 시 DamageEffect를 생성하고 적용
4. 데미지 계산은 ExecCalc 클래스에서 수행

---

## 데미지 및 전투 시스템

### Damage 흐름
- 투사체 생성 시 `FDamageEffectParams`를 통해 소스/타겟 정보, 데미지, 버프/디버프 정보를 전달
- 충돌 시 `GESpec`을 생성하고 TargetASC에 적용
- ExecCalc 클래스에서:
  - 기본 데미지 계산
  - 방어력 및 속성 저항 적용
  - 크리티컬, 상태 이상 확률 적용
  - RadialDamage 및 디버프 확률 적용

- 최종 데미지는 `AttributeSet::PostExecute`에서 적용되며 이때 HitResult, Knockback, Die 처리까지 이뤄짐

---

## 레벨업 및 경험치 시스템

- 몬스터 처치 → `XpReward` 태그를 통해 `GA_ListenForEvent` 발동
- 경험치 누적 → MaxXP 도달 시 레벨업
- `DA_LevelUpInfo`에 정의된 데이터 기반으로 `AttributePoint`, `SpellPoint` 지급
- MMC 클래스가 레벨 기반으로 MaxHealth, MaxMana 재계산 수행

---

## 인벤토리 및 아이템 시스템

- 아이템 유형: 소비형, 컬렉션형, 상점 구매 가능 아이템
- 아이템 획득 경로: 몬스터 처치, 보스 처치 후 상자 생성, 상점 구입
- 소비형 아이템은 단축키로 사용 가능하며 GE로 자신에게 효과 적용
- 데이터는 `AuraPlayerState`에서 관리
- UI는 MVC 패턴: `DataClass → Controller → UIWidget` 구조
- 아이템 수량 변화는 OnRep → 브로드캐스트 → 컨트롤러 → UI 반영 순으로 전달됨

---

## UI 시스템

- 모든 UI는 MVC 패턴으로 관리됩니다.
  - Controller는 데이터의 변화를 Listen하고 UI 위젯에 전달
  - 예: `AuraAttributeSet → AttributeMenuWidgetController → AttributeMenuWidget`
- UI에서 버튼 클릭 → Controller 함수 호출 → Data 처리 → 브로드캐스트 → UI 반영

---

## Enemy AI 시스템

- 언리얼 Behavior Tree 시스템 기반
- 주요 판단 요소:
  - 타겟 탐색 여부
  - HitReact, Stun 등 상태 이상 여부
  - 거리 기반 공격 가능 여부
- 행동: 이동, 대기, 근거리/원거리 공격, 소환 등

---

## 맵 전환 및 스트리밍 시스템

- `MapEntrance` 오버랩 시 서버에서 다음 맵을 비동기 로드
- `PlayerStartTag`에 따라 플레이어 위치 지정
- 클라이언트에는 RPC로 해당 맵 로드 요청
- 기존 맵은 자동으로 언로드 처리

---

## CheckPoint 및 세이브 시스템

- `SavePoint`: 오버랩 시 위치 저장
- `MapEntrance`: 맵 이동 + 위치 저장
- 사망 시 마지막 저장 위치로 리스폰
- 서버가 AWS를 통해 저장 요청

---

## AWS 연동 구조

### 사용 서비스
- Cognito (회원가입, 로그인, 세션 관리)
- DynamoDB (데이터 저장 및 불러오기)

### 테이블 구성
- `UserInfo`, `PlayerData`, `Attributes`, `Inventory`, `Skills`
- PrimaryKey는 `Cognito`의 `sub` 값을 사용

### 주요 매니저 클래스
- `HTTPRequestManager`: HTTP 요청/응답 처리
- `PortalManager`: 회원가입, 로그인, 이메일 인증
- `GameSessionManager`: Gamelift 세션 생성 및 관리
- `SaveManager`: 플레이어 데이터 저장 및 로딩

### 흐름
1. SignUp + Confirm → `sub` 생성 및 DynamoDB에 초기값 저장
2. SignIn → 위치 및 레벨 정보 불러오기
3. Join → GameSession 및 PlayerSession 생성 → 서버 접속
4. 플레이 중 CheckPoint 시점에 SaveManager를 통해 AWS에 저장

---

## 설계 원칙

- **Low Coupling**
  - 모듈 간 단방향 참조
  - 인터페이스 사용 (`AttributeSet → IPlayerInterface` 등)

- **MVC UI 구조**
  - 데이터 → 컨트롤러 → UI 바인딩
  - 델리게이트 및 브로드캐스트를 통한 변화 전달

- **확장성 있는 GAS 설계**
  - 모든 GA는 상속 기반으로 분리 구성
  - `GameplayTag`와 `DataAsset`로 유연한 구성 가능

---

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