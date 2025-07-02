
# Aura RPG - UE5.5 기반 RPG 시스템 포트폴리오

🎮 **RPG 게임 제작 포트폴리오**  
본 프로젝트는 **Unreal Engine 5.5**에서 **GAS(Gameplay Ability System)**, **Dedicated Server**, **AWS 연동**, **Streaming Level**, **AI 및 UI 시스템**을 활용한 **멀티플레이 RPG 게임** 개발 사례입니다.

> 🎥 **플레이 영상**: [https://youtu.be/4x9iSUawjKQ](https://youtu.be/4x9iSUawjKQ)  
> 📚 참고 강의: Udemy - Stephen Ulibarri님의 GAS 강의

---

## 💡 주요 특징

- **UE5.5 + GAS 기반 액션 RPG**
- **Dedicated Server 구조**
- **AWS Cognito + DynamoDB 연동 (Save/Load, Session 관리)**
- **MVC UI 아키텍처 적용**
- **AI, Navigation, Enhanced Input 등 언리얼 시스템 적극 활용**
- **Streaming Level 기반 맵 전환**

---

## 📦 프로젝트 구조 요약

- `Aura` 게임 모듈
  - GAS, EnhancedInput, UI, AI, Level, Skill, Attribute 등 대부분의 게임 시스템
- `DedicatedServer` 모듈
  - AWS 연동 및 플레이어 데이터 처리

---

## ⚔️ GAS 시스템 구성

### 1. **Attribute 설계**

- `PrimaryAttribute`: Strength, Intelligence, Vigor, etc
- `SecondaryAttribute`: MaxHealth, MaxMana 등 (Primary에 의해 계산됨)
- `SetByCaller` 방식으로 EventEffect를 통해 적용됨
- `ModMagnitudeCalculation` 사용 (`MMC_MaxHealth`, `MMC_MaxMana` 등)

### 2. **Skill 시스템**

- 구현 스킬:
  - **Active**: FireBolt, Electrocute, ArcaneShard
  - **Passive**: Siphon (흡수 계열)
- `GA_Base → GA_Targeted → GA_Damage → GA_Projectile → GA_SkillName` 구조로 상속 및 기능 분리
- `AbilityTag`로 키 매핑 → `Targeting → Montage → Notify → Projectile → DamageEffect`로 흐름

### 3. **데미지 처리 흐름**

- Projectile → Overlap → `GESpec` 생성 → `TargetASC`에 적용
- `ExecCalc`에서 데미지, 저항, 크리티컬, 디버프까지 계산
- 결과는 `AttributeSet::PostExecute`에서 처리 (Die, Hit, Knockback 등 반영)

---

## 🎮 게임 플레이 흐름

### 1. **레벨업 & Attribute 계산**

- 적 처치 → `XpReward` 전달 → `GA_ListenForEvent` 발동
- XP 누적 → 레벨업 → `MaxHealth`, `MaxMana` 등 재계산 (`MMC` 호출)

### 2. **스킬 장착 및 강화**

- `SpellPoint`로 스킬 활성화 및 강화
- 장착된 스킬은 키 입력으로 사용 가능

### 3. **인벤토리 시스템**

- `AuraPlayerState`에서 아이템 보유 정보 관리
- `Consumable`, `Collectible`, `StoreItem` 구분
- 아이템 사용 → GE로 적용 (회복 등)
- UI: `HUD → WidgetController → Data` 구조

---

## 🤖 Enemy AI

- Behavior Tree 기반 동작
  - 타겟 탐색, 사거리 판단, 상태 체크 (Stun, HitReact 등)
- 다양한 몬스터 스킬 구현 (근거리, 원거리, Firebolt, Summon 등)

---

## 🌍 맵 이동 & 스트리밍

- `MapEntrance` 오버랩 시 서버가 대상 맵 스트리밍 로드
- `PlayerStartTag`에 따라 캐릭터 위치 지정
- 클라 RPC 통해 클라이언트도 동일 맵 로드

---

## 💾 Checkpoint & Save/Load 시스템

- SavePoint: 위치 저장
- MapEntrance: 다음 맵 이동 + 저장
- 사망 시 마지막 SavePoint에서 재시작

---

## ☁️ AWS 연동 (DedicatedServer 모듈)

### 테이블 구조 (DynamoDB)

- `UserInfo`, `PlayerData`, `Attributes`, `Inventory`, `Skills`  
  → 공통 PrimaryKey: `userId` (Cognito `sub`)

### 구성 매니저

- `HTTPRequestManager`: 공통 HTTP 처리
- `PortalManager`: SignUp, Confirm 등 계정 인증
- `GameSessionManager`: AWS Gamelift 기반 세션 관리
- `SaveManager`: 게임 데이터 저장/불러오기

### 흐름 요약

1. **SignUp 후 Confirm**  
   → `sub` 생성 → DynamoDB에 초기 데이터 생성

2. **SignIn**  
   → 위치, 레벨, 상태 등 로드

3. **Join Game**  
   → `GameSession` 생성 → `PlayerSession` 발급 → 서버 OpenLevel로 입장

4. **Save**  
   → SavePoint/사망 시 서버가 DynamoDB에 저장 요청

---

## 🧠 설계 원칙 및 패턴

- **Low Coupling, High Cohesion**
  - 단방향 참조 유지
  - 인터페이스 기반 참조 (예: `AttributeSet → PlayerInterface`)

- **MVC 아키텍처 적용 (특히 UI 바인딩)**
  - `DataClass → Controller → Widget`
  - Broadcast 기반 상태 변화 전달

- **GameplayTag & DataAsset 기반 GAS 구성**

---

## ✅ 요약

| 항목 | 설명 |
|------|------|
| 엔진 | Unreal Engine 5.5 |
| 시스템 | GAS, AI, Navigation, Streaming Level, AWS 연동 |
| 네트워크 | Dedicated Server + Unreal Networking |
| 저장 방식 | AWS Cognito + DynamoDB |
| 디자인 패턴 | MVC, 인터페이스 기반 저커플링 설계 |
| 참고 강의 | Stephen Ulibarri GAS 강의 |

---

## 📺 플레이 영상

[▶ 영상 보러가기](https://youtu.be/4x9iSUawjKQ)
