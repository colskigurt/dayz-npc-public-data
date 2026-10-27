# DayZ NPC 공개 GitHub Raw 미러 게시·검증

2026-10-28 KST. 게시·일반 HTTP 검증은 완료했으며 외부 ChatGPT 웹 도구의 읽기는 Cache miss로 미검증이다.

## 1. 작업 목표

기존 사람용 대시보드를 유지하면서 AI용 공개 JSON·Markdown과 선정된 함수/대표 로그를 별도 GitHub Raw 경로로 제공한다. 중복 저장소1은 정리하고 원래 저장소 하나만 사용한다. 내부 원시 기록·설치 자산·자격증명·게임 코드 전체는 공개하지 않는다.

## 2. 실제 수행

공개 저장소가 존재해도 연결 도구 목록에는 나타나지 않던 상태에서 실제 쓰기는403 Resource not accessible by integration이었다. 사용자 화면으로 GitHub App 미설치가 확인됐고, 사용자가 별도 앱 설치를 완료한 뒤3개 저장소 목록과 공개 저장소 쓰기가 정상화됐다. 계정 로그인과 저장소 생성, 앱 설치·접근 권한을 구분해야 한다. 앞으로 목록이 비었다는 이유만으로 저장소를 다시 만들도록 안내하지 않는다.

사용자가 dayz-npc-public-data1을 삭제했고 인증된 조회에서404로 재확인했다. 원래 dayz-npc-public-data에16개 공개 파일을 일괄 커밋했다. 기존 JSON·선정 패킷은 바이트를 보존하고 README/AI_REVIEW/manifest의 미러 안내만 파생했다. 이후 검증 결과를 verification/latest.json으로 별도 게시했다.

## 3. 측정 결과

- 공개 데이터 커밋: 6aa211b92d542b9b34f888986b756de2950b4f12.
- snapshotId: snapshot-20261027T185443179320Z-acd7bf8c.
- 자료 generatedAt: 2026-10-27T18:54:43.179320Z. 새 게임 실험을 수행한 시각이 아니다.
- main Raw URL의 전체16파일: 인증 없는 GET200, 파일별 SHA-256·길이 일치, JSON3종 파싱/스키마 통과, snapshotId/시각 일치. 기본 urllib와 명시적 UA 모두 통과.
- 같은 데이터 커밋을 고정한 Raw URL도 전체 검사를 통과했다. 이는 브랜치 변경과 무관한 재현 경로다.
- 미국 Buffalo AS36352, 독일 Falkenstein AS24940, 일본 Tokyo AS3258의 독립 프로브3개가 latest.md에 HTTPS/IPv4/GET200을 받았다. 세 응답 모두 text/plain; charset=utf-8, 잘림 없음, 줄바꿈 정규화 후 본문 전체 일치. 각 프로브1회로 장기 가용성을 뜻하지 않는다.
- 외부 ChatGPT 웹 도구: 새 Raw AI_REVIEW.md/status.json/latest.md 모두 Cache miss. GitHub 저장소 페이지·blob 페이지·커밋 고정 Raw 대조도 Cache miss. 이 오류에는 원본 HTTP 상태가 제공되지 않았다.
- 기존 Sites는 public/version6 그대로다. 이번 게시에서 사이트 재배포·대시보드 수정·게임 실험은 하지 않았다.

NPC8/64 동작 안정성, frame-time p50/p95/p99, 실제 표시 품질은 이 묶음에서 새로 측정하지 않았다. 기존34개 지표의 근거 범위와 미수용 상태를 유지한다.

## 4. 원인 분석

확인된 사실: 앱 설치·권한 부재로 쓰기가 거절됐고 사용자 설치 후 같은 도구의 쓰기가 성공했다. Raw는 기본 urllib와3개 외부 네트워크에서 읽힌다. 기존 chatgpt.site에서 재현되던 urllib1010 차단은 이번 Raw 경로의 표본에서는 재현되지 않았다.

미확정: 이전에 읽혔던 GitHub의 오래된 테스트 fixture와 새 공개 Raw URL은 웹 도구 캐시 조건이 다를 수 있다. 현재 새 URL의 Cache miss가 캐시·색인·접근 정책 중 무엇 때문인지는 확인하지 못했다. 따라서 B 호스팅이 모든 ChatGPT 세션의 읽기를 해결했다고 주장하지 않는다. HTTP 성공과 특정 AI 웹 도구의 읽기는 별도 판정이다.

반증 범위: Raw에 파일이 없거나 비공개여서 일반 HTTP가 실패한다는 가설은 실제 전체GET·3개 외부 네트워크 결과에서 배제됐다. 그러나 모든 지역·모든 클라이언트의 성공을 뜻하지 않는다.

## 5. 검증 수준

게시 성공: GitHub main 갱신 완료. HTTP/스키마/전체 해시 회귀 통과: main과 커밋 고정 URL. 외부 네트워크 진단 성공:3개 ASN의 전체 Markdown 일치. 외부 AI URL 읽기 최종 수용: 미완료. NPC 기능·시각 품질·20ms p95 성능 목표 달성: 이 게시 작업으로 판정하지 않는다.

## 6. 변경 사항

새 공개 경로는 colskigurt/dayz-npc-public-data/main이다. 기존 대시보드와 private NorthFrontierAI 저장소는 수정하지 않았다. 공개 데이터16개와 별도 운영 검증 기록·이 보고서만 추가했다. 원본 실험 기록과 과거 보고서는 덮어쓰지 않았다.

manifest의 publicationAtExport/externalAIAtExport는 내보내기 당시 상태다. 현재 검증은 verification/latest.json을 읽는다. staleAfter는 실제 근거 시각 기준이며 게시/문서 생성이 신선한 실험을 뜻하지 않는다.

## 7. 원시 증거

실행 ID: github-mirror-20261028. prepared.json, payload/manifest.json, http-main-first.json, http-immutable.json, publication-receipt.json, verification-final.json, Globalping 생성/종료 응답, external-networks.json 및 게시 스크립트를 로컬에 보존한다. 기존 checkpoints 게시 함수로 저장 전 SHA-256과 파일 안정성을 검사한다. 공개 기록에는 개인 키·계정 이메일·토큰·로컬 경로·프로세스 정보·원격 프로브 상세 좌표를 넣지 않는다.

- [사람용 대시보드](https://dayz-npc-observatory.sappyvole8.chatgpt.site/)
- [AI 단일 검토 문서](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/AI_REVIEW.md)
- [최신 실험 요약](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/latest.md)
- [상태 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/status.json)
- [실험 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/experiments.json)
- [지표 JSON](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/api/public/metrics.json)
- [검증 기록](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/verification/latest.json)

## 8. 잔여 문제

외부 ChatGPT 도구의 새 URL Cache miss는 미해결이다. 다른 세션이 URL을 읽었는지 별도 결과가 필요하며 시간 경과만으로 해결됐다고 가정하지 않는다. 필요하면 일반 HTTP로 받은 AI_REVIEW.md 텍스트를 그대로 전달할 수 있다.

향후 의미 있는 실험 완료 때 기존 checkpoint를 작성한 뒤, 허용된 공개 파생본만 검증해 GitHub 미러를 갱신한다. 새 결과를 실제 HTTP와 SHA/스키마로 대조하고 외부 AI 읽기 상태를 별도 기록한다. 기본 대시보드는 유지하며 접근 오류를 고치기 위해 게임 파이프라인을 바꾸지 않는다.

